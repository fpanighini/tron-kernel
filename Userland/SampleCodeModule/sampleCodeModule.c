/* sampleCodeModule.c */


typedef struct {
	unsigned char b;
	unsigned char g;
	unsigned char r;
}Color;


Color black = {0x00, 0x00, 0x00};
Color white = {0xFF, 0xFF, 0xFF};
Color green = {0x1F, 0xED, 0x11};
Color gray = {0x90, 0x90, 0x90};
Color blue = {0xFF, 0x00, 0x00};
Color red = {0x00, 0x00, 0xFF};


long sys_read(unsigned char fd, char * s, unsigned int count);
long sys_write(char * s, Color c);
char * v = (char*)0xB8000 + 79 * 2;

// static int var1 = 0;
// static int var2 = 0;


int main() {
    ////All the following code may be removed 
    //*v = 'X';
    //*(v+1) = 0x74;

    ////Test if BSS is properly set up
    //if (var1 == 0 && var2 == 0)
    //    return 0xDEADC0DE;
    //
    //
    // sys_write("\n[Userland]\n", green);


    // char str[10];
    // char * string = str;

    // sys_read(0, string,10);

    // sys_write(string, red);
    //
    while(1){

    }


    return 100;
}
