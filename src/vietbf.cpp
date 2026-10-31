#include "brainfuck.hpp"

#include <fcitx-utils/key.h>
#include <fcitx-utils/keysym.h>
#include <fcitx-utils/keysymgen.h>
#include <fcitx-utils/textformatflags.h>
#include <fcitx/addonfactory.h>
#include <fcitx/event.h>
#include <fcitx/inputcontext.h>
#include <fcitx/inputmethodengine.h>
#include <fcitx/inputpanel.h>
#include <fcitx/text.h>
#include <fcitx/userinterface.h>

#include <string>
#include <unordered_map>

namespace {

class VietBFEngine final : public fcitx::InputMethodEngineV2 {
public:
    void keyEvent(const fcitx::InputMethodEntry &, fcitx::KeyEvent &event) override {
        if (event.isRelease()) {
            return;
        }

        auto *ic = event.inputContext();
        auto &pending = pending_[ic];
        const auto &key = event.key();

        // Backspace edits the one-character preedit. With no preedit, let the
        // application handle it normally.
        if (key.sym() == FcitxKey_BackSpace) {
            if (!pending.empty()) {
                pending.clear();
                updatePreedit(ic, pending);
                event.filterAndAccept();
            }
            return;
        }

        if (key.sym() == FcitxKey_Escape && !pending.empty()) {
            pending.clear();
            updatePreedit(ic, pending);
            event.filterAndAccept();
            return;
        }

        // Do not steal shortcuts. If a code is half-entered, finish it as a
        // plain character before allowing the shortcut through.
        const auto states = key.states();
        if (states.testAny(fcitx::KeyStates{
                fcitx::KeyState::Ctrl,
                fcitx::KeyState::Alt,
                fcitx::KeyState::Super,
                fcitx::KeyState::Super2,
                fcitx::KeyState::Hyper})) {
            flushPending(ic, pending);
            return;
        }

        const std::string typed = fcitx::Key::keySymToUTF8(key.sym());
        if (typed.size() != 1 ||
            static_cast<unsigned char>(typed[0]) < 0x20 ||
            static_cast<unsigned char>(typed[0]) > 0x7e) {
            flushPending(ic, pending);
            return;
        }

        const char ch = typed[0];
        const bool isAsciiLetter =
            (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
        const bool isToneDigit = ch >= '0' && ch <= '5';

        if (pending.empty()) {
            if (isAsciiLetter) {
                pending.assign(1, ch);
                updatePreedit(ic, pending);
                event.filterAndAccept();
            }
            // Non-letters (including digits, whitespace and punctuation) are
            // ordinary text unless they complete a pending Brainfuck code.
            return;
        }

        if (isToneDigit) {
            std::string pair = pending;
            pair.push_back(ch);
            pending.clear();
            updatePreedit(ic, pending);

            const auto result = brainfuck_.run(pair);
            if (!result.empty()) {
                ic->commitString(result);
            }
            event.filterAndAccept();
            return;
        }

        // A new letter before a tone digit means the previous letter was a
        // plain character (suffix 0). Start a new pair with this letter.
        if (isAsciiLetter) {
            flushPending(ic, pending);
            pending.assign(1, ch);
            updatePreedit(ic, pending);
            event.filterAndAccept();
            return;
        }

        // A delimiter or other printable character closes the pending letter
        // as <letter>0, then is passed through untouched to the client.
        flushPending(ic, pending);
    }

    void reset(const fcitx::InputMethodEntry &, fcitx::InputContextEvent &event) override {
        clearContext(event.inputContext());
    }

    void deactivate(const fcitx::InputMethodEntry &, fcitx::InputContextEvent &event) override {
        clearContext(event.inputContext());
    }

private:
    void updatePreedit(fcitx::InputContext *ic, const std::string &pending) {
        auto &panel = ic->inputPanel();
        panel.reset();
        if (!pending.empty()) {
            fcitx::Text text;
            text.append(pending, fcitx::TextFormatFlag::HighLight);
            text.setCursor(pending.size());
            if (ic->capabilityFlags().test(fcitx::CapabilityFlag::Preedit)) {
                panel.setClientPreedit(text);
            } else {
                panel.setPreedit(text);
            }
        }
        ic->updatePreedit();
        ic->updateUserInterface(fcitx::UserInterfaceComponent::InputPanel);
    }

    void flushPending(fcitx::InputContext *ic, std::string &pending) {
        if (pending.empty()) {
            return;
        }
        std::string pair = pending + '0';
        pending.clear();
        updatePreedit(ic, pending);
        const auto result = brainfuck_.run(pair);
        if (!result.empty()) {
            ic->commitString(result);
        }
    }

    void clearContext(fcitx::InputContext *ic) {
        if (!ic) {
            return;
        }
        pending_.erase(ic);
        updatePreedit(ic, "");
    }

    vietbf::Brainfuck brainfuck_;
    std::unordered_map<fcitx::InputContext *, std::string> pending_;
};

class VietBFEngineFactory final : public fcitx::AddonFactory {
public:
    fcitx::AddonInstance *create(fcitx::AddonManager *) override {
        return new VietBFEngine;
    }
};

} // namespace

FCITX_ADDON_FACTORY(VietBFEngineFactory);
