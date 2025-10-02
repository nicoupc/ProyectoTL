#include "MyForm.h"
#include "Presentacion.h"

using namespace ProyectoTL;

int main()
{
    // Mostrar la ventana de presentaci�n primero
    Presentacion^ presentacion = gcnew Presentacion();
    presentacion->Show();
    presentacion->Update(); // Forzar la actualizaci�n visual

    // Esperar 3 segundos (3000 milisegundos)
    System::Threading::Thread::Sleep(3000);

    // Cerrar la ventana de presentaci�n
    presentacion->Close();

    // Ejecutar la aplicaci�n principal
    Application::Run(gcnew MyForm());
    return 0;
}