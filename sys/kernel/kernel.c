// Простое ядро ОС
#define VIDEO_MEMORY 0xB8000
#define WIDTH 80
#define HEIGHT 25

// Цвета для текста
enum vga_color {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6,
    LIGHT_GRAY = 7,
    DARK_GRAY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_CYAN = 11,
    LIGHT_RED = 12,
    LIGHT_MAGENTA = 13,
    YELLOW = 14,
    WHITE = 15
};

// Позиция курсора
int cursor_x = 0;
int cursor_y = 0;

// Текущий цвет текста
unsigned char text_color = (BLACK << 4) | (LIGHT_GRAY & 0x0F);

// Функция для установки цвета текста
void set_color(enum vga_color fg, enum vga_color bg) {
    text_color = (bg << 4) | (fg & 0x0F);
}

// Функция для вывода символа
void putchar(char c) {
    unsigned short* video_memory = (unsigned short*)VIDEO_MEMORY;
    
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c == '\t') {
        cursor_x = (cursor_x + 4) & ~3;
    } else {
        unsigned short entry = (text_color << 8) | c;
        video_memory[cursor_y * WIDTH + cursor_x] = entry;
        cursor_x++;
    }
    
    // Проверка переполнения экрана
    if (cursor_x >= WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }
    
    if (cursor_y >= HEIGHT) {
        // Прокрутка экрана
        for (int i = 0; i < (HEIGHT - 1) * WIDTH; i++) {
            video_memory[i] = video_memory[i + WIDTH];
        }
        
        // Очистка последней строки
        for (int i = (HEIGHT - 1) * WIDTH; i < HEIGHT * WIDTH; i++) {
            video_memory[i] = (text_color << 8) | ' ';
        }
        
        cursor_y = HEIGHT - 1;
    }
}

// Функция для вывода строки
void print(const char* str) {
    while (*str) {
        putchar(*str++);
    }
}

// Простая функция задержки
void delay() {
    for (volatile int i = 0; i < 1000000; i++);
}

// Функция для очистки экрана
void clear_screen() {
    unsigned short* video_memory = (unsigned short*)VIDEO_MEMORY;
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        video_memory[i] = (text_color << 8) | ' ';
    }
    cursor_x = 0;
    cursor_y = 0;
}

// Точка входа ядра
void kernel_main() {
    clear_screen();
    set_color(WHITE, BLUE);
    
    print("=== YOS Kernel Loaded ===\n\n");
    
    set_color(GREEN, BLACK);
    print("Welcome to YaoiOS!\n");
    print("Kernel successfully initialized.\n\n");
    
    set_color(YELLOW, BLACK);
    print("System Information:\n");
    print("- Basic VGA text mode\n");
    print("- Simple memory management\n");
    print("- Basic I/O functions\n\n");
    
    set_color(LIGHT_CYAN, BLACK);
    print("Demo: Counting...\n");
    
    for (int i = 1; i <= 10; i++) {
        print("Count: ");
        
        // Простой вывод чисел
        if (i < 10) putchar('0');
        putchar('0' + i);
        print("\n");
        
        delay();
    }
    
    set_color(LIGHT_RED, BLACK);
    print("\nSystem ready. Enjoy! :)\n");
    
    // Бесконечный цикл
    while (1) {
        // Простая анимация
        set_color((cursor_x % 15) + 1, BLACK);
        putchar('*');
        delay();
    }
}
