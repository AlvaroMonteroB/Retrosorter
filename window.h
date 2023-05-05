#include<Windows.h>
#include<wingdi.h>
#include<vector>
using namespace std;

void show_image_window(const std::vector<unsigned char>& pixels, int width, int height,int channel){
    BITMAPINFO bmi={0};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;  // Signo negativo para mostrar la imagen correctamente
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 24;  // RGB de 24 bits
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = width * height * channel;
    HDC hdc = GetDC(NULL);  // Obtener el dispositivo de contexto de la pantalla principal
    HBITMAP hbm = CreateDIBitmap(hdc, &bmi.bmiHeader, CBM_INIT, pixels.data(), &bmi, DIB_RGB_COLORS);  // Crear el bitmap DIB
    HWND hwnd = CreateWindowA("STATIC", NULL, WS_VISIBLE | WS_POPUP | WS_SYSMENU, 0, 0, width, height, NULL, NULL, NULL, NULL);
    HDC hDC = GetDC(hwnd);  // Obtener el dispositivo de contexto de la ventana
    HDC memDC = CreateCompatibleDC(hDC);  // Crear un dispositivo de contexto compatible
    HGDIOBJ oldbmp = SelectObject(memDC, hbm);  // Seleccionar el bitmap en el contexto de memoria
    BitBlt(hDC, 0, 0, width, height, memDC, 0, 0, SRCCOPY);  // Copiar el bitmap en el contexto de la ventana

    // Liberar recursos
    SelectObject(memDC, oldbmp);
    DeleteDC(memDC);
    DeleteObject(hbm);
    ReleaseDC(hwnd, hDC);
    ReleaseDC(NULL, hdc);
    DestroyWindow(hwnd);
}