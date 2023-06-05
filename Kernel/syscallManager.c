#include "include/pipe.h"
#include "include/scheduler.h"
#include "include/videoDriver.h"
#include <syscallManager.h>

int getKbdBuffer(char *buf, uint32_t count, int *pos);
extern void outb(int par1, uint8_t par2);
extern uint8_t inb(uint8_t par1);

void playSound(uint32_t frequency);
void noSound();
void beep1(uint32_t frequency);
void beep2();

extern uint64_t registers[REGISTER_NUM];

// TODO: checkear el uso de write con pipes
uint64_t sys_write(uint8_t fd, char *string, uint64_t n, Color color)
{
    if (fd == STDERR)
    {
        color = RED;
        fd = 1;
    }

    uint64_t pipe_fd = get_current_write();

/*    
    printString("{", WHITE);
    printBase(sys_get_pid(), 10);
    printString("}", WHITE);

    if(pipe_fd != 1) { 
        printString("pipe_fd: ", WHITE);
        printBase(pipe_fd, 10);
    }
    */
    
    // Attempting to write on stdin
    if (fd == 0)
    {
        return 0;
    }

    // Attempting to write 0 bytes to a pipe
    if (fd != 1 && n == 0)
    {
        return 0;
    }

    // Printing to stdout
    if (pipe_fd == 1)
    {
        printString((uint8_t *)string, color);
        return 0;
    }

    // Writing n bytes to pipe
    uint64_t ret = pipe_write(pipe_fd - 3, string, n);
    return ret;
}

int getKbdBuffer(char *buf, uint32_t count, int *pos)
{
    if (getCount())
    {
        *pos += readBuf(buf + *pos, count - *pos - 1);
        clearKeyboardBuffer();
        return 1;
    }
    return 0;
}

// TODO: checkear el uso de read con pipes
uint64_t sys_read(uint8_t fd, char *buf, uint32_t count)
{
    uint64_t new_fd = 0;
    if (fd == 0 || fd == 1 || fd == 2)
    {
        new_fd = get_current_read();
        // TODO: ver CAT
        // printString("new_fd: ", WHITE);
        // printBase(count, 10);
    }
    else
    {
        new_fd = fd;
    }

    if (new_fd == 1 || new_fd == 2)
    {
        return 0;
    }

    if (new_fd != 0)
    {
        uint64_t ret = pipe_read(new_fd - 3, buf , count);
        return ret;
    }

    int i = 0;
    int read = 0;
    clearKeyboardBuffer();
    while (i < count - 1)
    {
        read = getKbdBuffer(buf, count, &i);
        if (read && (buf[i - 1] == '\n' || buf[i - 1] == 0))
        {
            //buf[i - 1] = 0;
            buf[i - 1] = '\n';
            return i;
        }
        _hlt();
    }
    buf[i] = 0;
    return i;
}

uint64_t sys_timedRead(uint8_t fd, char * buf, uint32_t count, uint32_t millis)
{
    if (fd != get_current_read())
        return 0;

    int i = 0;
    int read = 0;
    clearKeyboardBuffer();
    buf[0] = 0;
    int initial = milliseconds_elapsed();
    while ((i < count - 1) && ((milliseconds_elapsed() - initial) < millis))
    {
        read = getKbdBuffer(buf, count, &i);
        if (read && (buf[i - 1] == '\n' || buf[i - 1] == 0))
        {
            buf[i - 1] = 0;
            return i;
        }
        _hlt();
    }
    buf[i] = 0;
    return i;
}

uint64_t sys_time()
{
    return getTime();
}

uint64_t sys_date()
{
    return getDate();
}

uint64_t sys_clearScreen()
{
    clearScreen();
    return 0;
}

uint64_t sys_drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, Color color)
{
    fillrect((uint16_t)x, (uint16_t)y, (uint16_t)width, (uint16_t)height, color);
    return 0;
}

void playSound(uint32_t frequency)
{
    uint32_t Div;
    uint8_t tmp;

    // Set the PIT to the desired frequency
    Div = 1193180 / frequency;
    outb(0x43, 0xb6);
    outb(0x42, (uint8_t)(Div));
    outb(0x42, (uint8_t)(Div >> 8));

    // And play the sound using the PC speaker
    tmp = inb(0x61);
    if (tmp != (tmp | 3))
        outb(0x61, tmp | 3);
}

void noSound()
{
    uint8_t tmp = inb(0x61) & 0xFC;
    outb(0x61, tmp);
}

void beep1(uint32_t frequency)
{
    playSound(frequency);
    sys_wait(200);
    noSound();
}

void beep2()
{
    playSound(1193180);
    sys_wait(200);
    noSound();
}

uint64_t sys_beep(uint32_t frequency)
{
    beep1(frequency);
    sys_wait(100);
    beep2();
    return 0;
}

uint64_t sys_widthScr()
{
    return getWidth();
}

uint64_t sys_heightScr()
{
    return getHeight();
}

uint64_t sys_writeAt(uint16_t x, uint16_t y, char *string, Color color)
{
    printStringAt(x, y, (uint8_t *)string, color);
    return 0;
}

uint64_t sys_wait(uint32_t millis)
{
    int initial = milliseconds_elapsed();
    while (milliseconds_elapsed() - initial < millis)
        _hlt();

    return 0;
}

uint64_t sys_inforeg(uint64_t array[REGISTER_NUM])
{
    if (registers != 0)
    {
        for (int i = 0; i < REGISTER_NUM; i++)
        {
            array[i] = registers[i];
        }
        return 1;
    }
    return 0;
}

uint64_t sys_changeFontSize(uint32_t size)
{
    return changeFontSize(size);
}

void *sys_malloc(uint64_t memSize)
{
    return malloc(memSize);
}

void sys_free(void *ptr)
{
    free(ptr);
}

uint64_t sys_exec(char *name, void *program, char **argv, uint64_t read_fd, uint64_t write_fd, uint64_t priority)
{
    return add_process(name, program, argv, read_fd, write_fd, priority);
}

uint64_t sys_sem_open(char *name, int value)
{
    return sem_open(name, value);
}

uint64_t sys_sem_close(char *name)
{
    return sem_close(name);
}

uint64_t sys_sem_wait(char *name)
{
    return sem_wait(name);
}

uint64_t sys_sem_post(char *name)
{
    return sem_post(name);
}

uint64_t sys_sem_count()
{
    return get_sem_count();
}

uint64_t sys_sem_info(int idx, p_sem buffer)
{
    return get_sem_info(idx, buffer);
}

uint64_t sys_pipe_open(char *name)
{
    return (uint64_t)pipe_open(name);
}

uint64_t sys_pipe_close(int id)
{
    pipe_close(id);
    return 1;
}

uint64_t sys_pipes_info()
{
    char *response = pipes_info();
    return (uint64_t)response;
}

uint64_t sys_get_pid()
{
    return get_running_pid();
}

uint64_t sys_kill(uint64_t pid)
{
    return kill_process(pid);
}

uint64_t sys_change_priority(uint64_t pid, uint64_t priority)
{
    return change_priority(pid, priority);
}

uint64_t sys_block(uint64_t pid)
{
    return block_process(pid);
}

uint64_t sys_unblock(uint64_t pid)
{
    return ready_process(pid);
}

void sys_yield(void)
{
    force_current_yield();
}

void sys_ps(void)
{
    print_all_nodes();
}

