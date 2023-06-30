# 2-Distributed Computing

Mô phỏng tính toán phân tán sử dụng 2 máy tính đếm từ.


Quá trình thực hiện của mô phỏng cho nhiều máy khách:

```
                :   ... (more) ...    :
                |                     |
                |    _______  COUNT   |
                :--> CLIENT  ---------:
                |    _______          |
                |                     |
                |    _______  COUNT   |
                :--> CLIENT  ---------:
______  DIVIDE* |    _______          |   COMBINE*    _______
INPUT  ---------|                     :-------------> OUTPUT
______          |    _______          |               _______
                '--> SERVER ----------'
                     _______  COUNT*
                        
*: Process in server.
```

Để thực hiện mô phỏng cần sử dụng 2 máy tính: một máy cho máy chủ và một máy đóng vai trò máy khách. Thực hiện các bước như sau:

Sao chép mã nguồn trong thư mục `src` vào máy chủ và khách tương ứng với thư mục `src/server` và `src/client` kèm theo `Makefile`. Khi ở 2 máy tính mô phỏng đã có mã nguồn, thực hiện biên dịch chương trình như sau.

### Makefile

```bash
## Nếu sử dụng make trên các hệ điều hành linux
# Ở máy chủ
make server
# Ở máy khách
make client

## Nếu sử dụng mingw32-make thì tương tự
# Ở máy chủ
mingw32-make server
# Ở máy khách
mingw32-make client
```

Sau khi thực hiện biên dịch xong, thực hiện mô phỏng sẽ bắt đầu từ phía máy khách (máy khách đang rỗi và chờ đợi được giao việc).





### Client

Ở phía máy khách, các tác vụ để xử lý được điều khiển bởi `control` do đó chỉ cần gọi

```bash
./control SERVER_IP
# Ex: ./control 192.168.2.110
```

và chờ đợi việc nhận dữ liệu, tính toán, gửi trả kết quả hoàn tất.

### Server

Các tác vụ để thực hiện mô phỏng được điều khiển bởi `control_program` ở máy chủ. Để bắt đầu chạy

```bash
./control_program
# Prompt:
#     Enter the name of the input file: ../../data/fulltext.txt
```

Khi chương trình điều khiển chạy, bạn sẽ được hỏi về đường dẫn của tệp văn bản sử dụng trong mô phỏng đếm từ này. Sau khi nhập đường dẫn đến tệp, chương trình sẽ bắt đầu chia dữ liệu thành 2 phần và gửi cho máy khách một phần dữ liệu đồng thời máy chủ cũng thực hiện tính toán đếm từ và chờ kết quả trả về từ máy khách.

### Các chương trình xử lý

#### `data-divide` - Chia dữ liệu

Chia dữ liệu thành hai nửa với số lượng từ xấp xỉ nhau.

```bash
./data-divide INPUT OUTPUT1 OUTPUT2
# Ex: ./data-divide fulltext.txt divided1.txt divided2.txt
```



#### `combine` - Hợp nhất dữ liệu

Gộp kết quả tính toán từ các máy tính.

```bash
./combine INPUT1 INPUT2 OUTPUT
# Ex: ./combine count1.txt count2.txt count.txt
```



#### `wordcount` - Đếm từ

Đếm số lượng xuất hiện của mỗi từ có trong tệp văn bản.

```bash
./wordcount INPUT OUTPUT
# Ex: ./wordcount divided1.txt count1.txt
```



#### `client` - Nhận và gửi tệp từ phía khách 

Cầu nối truyền tệp giữa máy chủ và khách với cơ chế: tệp gửi lỗi sẽ được gửi lại cho tới khi thành công; kết nối với máy chủ thất bại sẽ thử kết nối lại sau một khoảng thời gian.

```bash
./client send FILENAME [SERVER_IP]
./client receive FILENAMETOSAVE [SERVER_IP]
# Ex: ./client send count2.txt 192.168.2.110
#     ./client receive divided2.txt 192.168.2.110
```



#### `server` - Nhận và gửi tệp từ phía máy chủ

Cầu nối truyền tệp giữa máy chủ và khách.

```bash
./server send FILENAME
./server receive FILENAMETOSAVE
# Ex: ./server send divided2.txt
#     ./server receive count2.txt 
```
