# Wind Waker File Parse and Converter
It does as the title suggests :)

### Building and Running
NOTE THAT THIS IS CURRENTLY ONLY KNOWN TO RUN ON MACOS. I would think it can run on Linux and Windows, but the commands for Windows may be different if you don't use WSL (Windows Subsystem for Linux).

This program requires a file called `JaiSeqs.arc` in the folder `data`. You will need to supply this file as I'm fairly certain I can't legally just publish it on the internet. It's really not hard to get (there's this wonderful thing called Google), but you will have to supply it yourself. Once you have it, you will need to place it in an adjecent folder called `data` (should be in the same directory as `src` and `bin`). Also, in `bin`, you will need a folder called `files`, and in `files`, you will need to create three folders: `bms`, `midi` (currently unused), and `txt`. In total, this should be the file tree:
```
root
  + bin
    + bms
    + midi
    + txt
  + data
    + JaiSeqs.arc
  + src
    + <source files>
  + <other files>
```
Currently, you can build by just running `make`. This will create a file called `get_stuff ` in the folder `bin`. You can then run this using `./get_stuff` (you will want to send this to a file), assuming you are in the `bin` folder. I.e.:
```bash
make
cd bin
./get_stuff > ./stuff
```
Once you have done this you can "convert" (it's just a text dump that's not fully complete) bms files. First, find a bms file you want to convert, and pass both `convert` and the file name (NO extension) as parameters. I.e.:
```bash
make
cd bin
./get_stuff convert <filename>
```
This will make a file named  `<filename>.ansi` in `bin/txt`.
