cc ex-8-1-cat-std.c -o cat-std
cc ex-8-1-cat.c -o cat-sys
dd if=/dev/zero of=file.txt count=1024 bs=262144
echo ""
echo "warming up..."
cat file.txt 
cat file.txt 
cat file.txt 
echo ""
echo "linux cat:"
time cat file.txt 
time cat file.txt 
time cat file.txt 
echo ""
echo "using system calls:"
time cat-sys file.txt 
time cat-sys file.txt 
time cat-sys file.txt 
echo ""
echo "using std lib:"
time cat-std file.txt 
time cat-std file.txt 
time cat-std file.txt 
rm file.txt 
rm cat-std 
rm cat-sys

