#include "MyForm.h"
#include "Presentacion.h"

using namespace ProyectoTL;

int main()
{
    // Mostrar la ventana de presentacion primero
    Presentacion^ presentacion = gcnew Presentacion();
    presentacion->Show();
    presentacion->Update(); // Forzar la actualizacion visual

    // Esperar 3 segundos (3000 milisegundos)
    System::Threading::Thread::Sleep(3000);

    // Cerrar la ventana de presentacion
    presentacion->Close();

    // Ejecutar la aplicacion principal
    Application::Run(gcnew MyForm());
    return 0;
}