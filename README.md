7zz 25.00 with additional options

## Use passwords list to open archive (new feature)

You can use `-fp` argument with selection passwords list
`./7zz x test.7z -fppasswords.txt`
7zz will try all passwords from list to open archive.

## Unpack files with mask in filename (new feature)

We must add characters "$?" to the end of a mask's pattern which means that searching starts from the tail of a file's full path

### for *NIX
`./7zz x test.7z -i'!*/*ense*.txt$?' -i'!*/*ip*.hhp$?' -i'!*.mak$?' -ssc-`

### for WINDOWS
`7zz.exe x test.7z -i!*/*ense*.txt$? -i!*/*ip*.hhp$? -i!*.mak$? -ssc-`

An example below also extracts necessary files from test.7z archive by original 7zz application.
Though some folders containing matching words are extracted as well.

`7zz.exe x test.7z -i!*\*example*.txt -i!*\*test*.txt -i!*.rar -i!*.zip -i!*.7z -r`

## Build instruction (Example for debian)
```bash
apt update -y
apt install make gcc libc-dev git -y
git clone https://github.com/keklick1337/7zip-extended
cd 7zip-extended/CPP/7zip/Bundles/Alone2
make -j -f makefile.gcc
cd _o
chmod 777 7zz
mv 7zz /bin/7zz
```