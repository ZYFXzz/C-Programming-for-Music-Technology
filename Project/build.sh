# #!/bin/sh
# gcc recplay.c -o recplay.exe  -DNCURSES_STATIC   -I/msys64/mingw64/include/ncurses  -L/usr/local/lib -lsndfile -lportaudio -lncurses
    
#!/bin/sh
gcc recplay.c -o project.exe -DNCURSES_STATIC -I/msys64/mingw64/include/ncurses -L/usr/local/lib -lsndfile -lportaudio -lncurses -lfftw3 -lm