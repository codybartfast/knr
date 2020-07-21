cc ex-8-2-fillbuf.c -o cat-fields
cc ex-8-2-fillbuf-bits.c -o cat-bits
dd if=/dev/zero of=file.txt count=1024 bs=2048
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
echo "using bits:"
time cat-bits file.txt 
time cat-bits file.txt 
time cat-bits file.txt 
echo ""
echo "using fields:"
time cat-fields file.txt
time cat-fields file.txt
time cat-fields file.txt
rm file.txt 
rm cat-bits
rm cat-fields


