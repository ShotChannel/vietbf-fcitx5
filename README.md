# fcitx5-vietbf

VietBF (Việt Brainfuck) là một bộ gõ tiếng Việt thực hiện bằng một VM chạy Brainfuck, cho Fcitx5.

### Disclaimer: Đây là một trò đùa (joke thôi hiểu ko) trong lúc t đang rảnh và thấy thừa tokens. Đây KHÔNG phải một dự án nghiêm túc và t sẽ không chắc chắn nó sẽ hoạt động trên máy bạn hay không. Nếu thực sự cần một bộ gõ tiếng Việt, hãy cài đặt https://github.com/LotusInputMethod/fcitx5-lotus .

## Cách gõ

VietBF dùng một phím chữ làm ký tự gốc và một phím số `0` đến `5` để chọn dạng chữ hoặc thanh điệu.

### Các phím nguyên âm

| Phím | Ký tự gốc | `0` | `1` | `2` | `3` | `4` | `5` |
|---|---|---|---|---|---|---|---|
| `a` | a | a | á | à | ả | ã | ạ |
| `A` | â | â | ấ | ầ | ẩ | ẫ | ậ |
| `B` | ă | ă | ắ | ằ | ẳ | ẵ | ặ |
| `e` | e | e | é | è | ẻ | ẽ | ẹ |
| `E` | ê | ê | ế | ề | ể | ễ | ệ |
| `i` | i | i | í | ì | ỉ | ĩ | ị |
| `o` | o | o | ó | ò | ỏ | õ | ọ |
| `O` | ô | ô | ố | ồ | ổ | ỗ | ộ |
| `u` | u | u | ú | ù | ủ | ũ | ụ |
| `U` | ư | ư | ứ | ừ | ử | ữ | ự |
| `R` | ơ | ơ | ớ | ờ | ở | ỡ | ợ |
| `y` | y | y | ý | ỳ | ỷ | ỹ | ỵ |

Các phím đặc biệt khác:

| Tổ hợp | Kết quả |
|---|---|
| `D0` | đ |
| `c0` | c |
| `n0` | n |
| `m0` | m |

Các phụ âm thông thường được tự động hoàn tất bằng mã `0` khi bạn gõ phím tiếp theo. Chỉ dùng các phím `1` đến `5` ngay sau một nguyên âm có hỗ trợ dấu. Không gõ số thanh điệu sau phụ âm.

### Ví dụ

Để gõ `chào bạn!`, nhấn lần lượt:

```text
c  h  a  2  o  Space  b  a  5  n  !
```

Trong đó:

- `c`, `h`, `b`, `n` được tự động hoàn tất thành phụ âm thường.
- `a` rồi `2` tạo thành `à`.
- `a` rồi `5` tạo thành `ạ`.
- `Space` và `!` được chuyển đến ứng dụng bình thường.

Kết quả: `chào bạn!`

### Hành vi khi nhập

- Sau khi nhấn một phím chữ, VietBF giữ chữ đó ở trạng thái **preedit** trong lúc chờ phím tiếp theo.
- Nhấn `0` đến `5` để hoàn tất cặp mã tương ứng.
- Nếu nhấn một chữ cái khác trước, chữ đang chờ được xuất dưới dạng thông thường bằng mã `0`; chữ mới trở thành chữ đang chờ.
- Dấu cách hoặc dấu câu sẽ hoàn tất chữ đang chờ rồi được gửi nguyên trạng đến ứng dụng.
- `Backspace` hủy chữ đang chờ; `Escape` cũng hủy thao tác đang nhập.
- Các tổ hợp phím có `Ctrl`, `Alt`, `Super`, `Hyper` không bị VietBF chiếm dụng.

**Lưu ý về chữ hoa:** Các phím viết hoa `A`, `B`, `D`, `E`, `O`, `R`, `U` là mã dành riêng cho chữ tiếng Việt đặc biệt. Bản hiện tại chưa hỗ trợ nhập chữ in hoa tiếng Anh thông thường.


## Cài đặt trên Arch Linux 

Cài đặt packages cần thiết:

```sh
sudo pacman -S --needed fcitx5 base-devel cmake ninja
```

Build: 

```sh
cmake -S . -B build -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=/usr
cmake --build build
sudo cmake --install build
```

Khởi động lại Fcitx5:

```sh
fcitx5 -rd
```

Mở **Fcitx5 Configuration**, thêm **VietBF (Brainfuck)** vào nhóm bộ gõ đang sử dụng, rồi chuyển sang VietBF như với các bộ gõ khác.

Addon được nạp bên trong tiến trình Fcitx5 hiện có. Không cần chạy một daemon VietBF riêng và cũng không cần quyền root sau khi cài đặt.

Nếu sau tất cả những điều trên mà nó vẫn dell hoạt động? Tốt, chúc mừng vì đã tốn thời gian trong cuộc đời m để tin cái dự án này nó thực sự hoạt động hahaha
