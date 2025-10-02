#pragma once

namespace ProyectoTL {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Resumen de MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			colorFigura = Color::Blue;
			//
			//TODO: agregar codigo de constructor aqui
			//


		// APLICAR ESTILOS MODERNOS
		AplicarEstilosModernos();

			// Deshabilitar todas las cajas de texto al inicio
			caja_Grados->Enabled = false;
			caja_Grados->BackColor = Color::FromArgb(240, 240, 240);
			grado_ubicacion1->Enabled = false;
			grado_ubicacion1->BackColor = Color::FromArgb(240, 240, 240);
			grado_ubicacion2->Enabled = false;
			grado_ubicacion2->BackColor = Color::FromArgb(240, 240, 240);
			btn_punto1X->Enabled = false;
			btn_punto1X->BackColor = Color::FromArgb(240, 240, 240);
			btn_punto2X->Enabled = false;
			btn_punto2X->BackColor = Color::FromArgb(240, 240, 240);
			btn_punto3X->Enabled = false;
			btn_punto3X->BackColor = Color::FromArgb(240, 240, 240);
			btn_punto3Y->Enabled = false;
			btn_punto3Y->BackColor = Color::FromArgb(240, 240, 240);
			btn_punto4X->Enabled = false;
			btn_punto4X->BackColor = Color::FromArgb(240, 240, 240);
			btn_punto4Y->Enabled = false;
			btn_punto4Y->BackColor = Color::FromArgb(240, 240, 240);
			btn_punto1Y->Enabled = false;
			btn_punto1Y->BackColor = Color::FromArgb(240, 240, 240);
			btn_punto2Y->Enabled = false;
			btn_punto2Y->BackColor = Color::FromArgb(240, 240, 240);

			// Deshabilitar campos de homotecia/escala
			txtVEscala->Enabled = false;
			txtVEscala->BackColor = Color::FromArgb(240, 240, 240);
			txtXEscala->Enabled = false;
			txtXEscala->BackColor = Color::FromArgb(240, 240, 240);
			txtYEscala->Enabled = false;
			txtYEscala->BackColor = Color::FromArgb(240, 240, 240);

			// Deshabilitar botones de transformaciones
			btn_Dibujar->Enabled = false; // Dibujar
			btn_rotar->Enabled = false; // Rotar
			btn_ejeX->Enabled = false; // Espejo X
			btn_ejeY->Enabled = false; // Espejo Y
			btn_ejeZ->Enabled = false; // Espejo Z
			CambiarColor->Enabled = false; // Cambiar color
			btnEscalar->Enabled = false; // Homotecia/Escalar

			// Configurar validacion numerica para todas las cajas de texto

			ConfigurarValidacionNumerica();
		}
		// Metodo para configurar validacion numerica
		void ConfigurarValidacionNumerica()
		{
			// Agregar event handlers para validacion de solo numeros
			btn_punto1X->KeyPress += gcnew KeyPressEventHandler(this, &MyForm::textBox_KeyPress);
			btn_punto1Y->KeyPress += gcnew KeyPressEventHandler(this, &MyForm::textBox_KeyPress);
			btn_punto2X->KeyPress += gcnew KeyPressEventHandler(this, &MyForm::textBox_KeyPress);
			btn_punto2Y->KeyPress += gcnew KeyPressEventHandler(this, &MyForm::textBox_KeyPress);
			btn_punto3X->KeyPress += gcnew KeyPressEventHandler(this, &MyForm::textBox_KeyPress);
			btn_punto3Y->KeyPress += gcnew KeyPressEventHandler(this, &MyForm::textBox_KeyPress);
			btn_punto4X->KeyPress += gcnew KeyPressEventHandler(this, &MyForm::textBox_KeyPress);
			btn_punto4Y->KeyPress += gcnew KeyPressEventHandler(this, &MyForm::textBox_KeyPress);
			grado_ubicacion1->KeyPress += gcnew KeyPressEventHandler(this, &MyForm::textBox_KeyPress);
			grado_ubicacion2->KeyPress += gcnew KeyPressEventHandler(this, &MyForm::textBox_KeyPress);
			caja_Grados->KeyPress += gcnew KeyPressEventHandler(this, &MyForm::textBox_KeyPress);
			
			// Agregar validacion para campos de homotecia/escala
			txtVEscala->KeyPress += gcnew KeyPressEventHandler(this, &MyForm::textBox_KeyPress);
			txtXEscala->KeyPress += gcnew KeyPressEventHandler(this, &MyForm::textBox_KeyPress);
			txtYEscala->KeyPress += gcnew KeyPressEventHandler(this, &MyForm::textBox_KeyPress);
		}

		// Event handler para validacion de entrada numerica
		void textBox_KeyPress(Object^ sender, KeyPressEventArgs^ e)
		{
			// Permitir numeros, signo negativo, punto decimal y teclas de control
			if (!Char::IsDigit(e->KeyChar) && e->KeyChar != '-' && e->KeyChar != '.' && !Char::IsControl(e->KeyChar))
			{
				e->Handled = true; // Rechazar el caracter
			}

			// Permitir signo negativo solo al inicio
			if (e->KeyChar == '-')
			{
				TextBox^ tb = safe_cast<TextBox^>(sender);
				if (tb->SelectionStart != 0 || tb->Text->Contains("-"))
				{
					e->Handled = true;
				}
			}

			// Permitir solo un punto decimal
			if (e->KeyChar == '.')
			{
				TextBox^ tb = safe_cast<TextBox^>(sender);
				if (tb->Text->Contains("."))
				{
					e->Handled = true;
				}
			}
		}

		// Metodo para ordenar puntos en sentido antihorario (para formar poligonos correctos)
		array<Point>^ OrdenarPuntosPoligono(array<Point>^ puntos)
		{
			if (puntos->Length <= 2) return puntos;

			// Calcular el centroide (punto central)
			double cx = 0, cy = 0;
			for (int i = 0; i < puntos->Length; i++) {
				cx += puntos[i].X;
				cy += puntos[i].Y;
			}
			cx /= puntos->Length;
			cy /= puntos->Length;

			// Crear una lista de puntos con sus angulos
			System::Collections::Generic::List<System::Tuple<double, Point>^>^ puntosConAngulo = 
				gcnew System::Collections::Generic::List<System::Tuple<double, Point>^>();

			for (int i = 0; i < puntos->Length; i++) {
				double angulo = Math::Atan2(puntos[i].Y - cy, puntos[i].X - cx);
				puntosConAngulo->Add(gcnew System::Tuple<double, Point>(angulo, puntos[i]));
			}

			// Ordenar manualmente usando bubble sort
			for (int i = 0; i < puntosConAngulo->Count - 1; i++) {
				for (int j = 0; j < puntosConAngulo->Count - i - 1; j++) {
					if (puntosConAngulo[j]->Item1 > puntosConAngulo[j + 1]->Item1) {
						// Intercambiar
						System::Tuple<double, Point>^ temp = puntosConAngulo[j];
						puntosConAngulo[j] = puntosConAngulo[j + 1];
						puntosConAngulo[j + 1] = temp;
					}
				}
			}

			// Extraer los puntos ordenados
			array<Point>^ puntosOrdenados = gcnew array<Point>(puntos->Length);
			for (int i = 0; i < puntos->Length; i++) {
				puntosOrdenados[i] = puntosConAngulo[i]->Item2;
			}

			return puntosOrdenados;
		}


		// Metodo para validar que los campos requeridos esten llenos
		bool ValidarCamposRequeridos()
		{
			if (cajaFormas->SelectedIndex == -1)
			{
				MessageBox::Show("Por favor seleccione el tipo de figura.", "Validacion", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				return false;
			}

			// Validar campos segun el tipo de figura
			if (cajaFormas->SelectedItem->ToString() == "Triangulo")
			{
				if (String::IsNullOrEmpty(btn_punto1X->Text) || String::IsNullOrEmpty(btn_punto1Y->Text) ||
					String::IsNullOrEmpty(btn_punto2X->Text) || String::IsNullOrEmpty(btn_punto2Y->Text) ||
					String::IsNullOrEmpty(btn_punto3X->Text) || String::IsNullOrEmpty(btn_punto3Y->Text))
				{
					MessageBox::Show("Por favor complete todas las coordenadas para el triangulo.", "Validacion", MessageBoxButtons::OK, MessageBoxIcon::Warning);
					return false;
				}
			}
			else if (cajaFormas->SelectedItem->ToString() == "Cuadrilatero")
			{
				if (String::IsNullOrEmpty(btn_punto1X->Text) || String::IsNullOrEmpty(btn_punto1Y->Text) ||
					String::IsNullOrEmpty(btn_punto2X->Text) || String::IsNullOrEmpty(btn_punto2Y->Text) ||
					String::IsNullOrEmpty(btn_punto3X->Text) || String::IsNullOrEmpty(btn_punto3Y->Text) ||
					String::IsNullOrEmpty(btn_punto4X->Text) || String::IsNullOrEmpty(btn_punto4Y->Text))
				{
					MessageBox::Show("Por favor complete todas las coordenadas para el cuadrilatero.", "Validacion", MessageBoxButtons::OK, MessageBoxIcon::Warning);
					return false;
				}
			}

			return true;
		}

		// Metodo para validar campos de rotacion
		bool ValidarCamposRotacion()
		{
			if (String::IsNullOrEmpty(caja_Grados->Text) || String::IsNullOrEmpty(grado_ubicacion1->Text) || String::IsNullOrEmpty(grado_ubicacion2->Text))
			{
				MessageBox::Show("Por favor complete todos los campos para la rotacion (grados y ubicacion).", "Validacion", MessageBoxButtons::OK, MessageBoxIcon::Warning);
				return false;
			}
			return true;
		}
	private:

	private: System::Windows::Forms::Button^ btn_ejeX;
	private: System::Windows::Forms::Button^ btn_ejeY;
	private: System::Windows::Forms::Button^ btn_ejeZ;
	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::Label^ label7;
	private: System::Windows::Forms::Label^ label8;
	private: System::Windows::Forms::Label^ label9;
	private: System::Windows::Forms::Label^ label10;
	private: System::Windows::Forms::Label^ label11;
	private: System::Windows::Forms::Label^ label12;
	private: System::Windows::Forms::Label^ label13;
	private: System::Windows::Forms::Label^ label14;
	private: System::Windows::Forms::Label^ label15;
	private: System::Windows::Forms::Label^ label16;
	private: System::Windows::Forms::Label^ label17;
	private: System::Windows::Forms::Label^ label18;
	private: System::Windows::Forms::Button^ CambiarColor;

	public:
	private:
		array<Point>^ figuraOriginal;
	private: System::Windows::Forms::Label^ label19;
	private: System::Windows::Forms::Label^ label20;
	private: System::Windows::Forms::Label^ label21;
	private: System::Windows::Forms::Label^ label22;
	private: System::Windows::Forms::TextBox^ txtVEscala;
	private: System::Windows::Forms::TextBox^ txtXEscala;
	private: System::Windows::Forms::TextBox^ txtYEscala;
	private: System::Windows::Forms::Button^ btnEscalar;
		   Color colorFigura;
	protected:
		/// <summary>
		/// Limpiar los recursos que se esten usando.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^ pictureBox1;
	protected:
	private: System::Windows::Forms::Button^ btn_demo;
	private: System::Windows::Forms::TextBox^ btn_punto1X;
	private: System::Windows::Forms::TextBox^ btn_punto1Y;
	private: System::Windows::Forms::TextBox^ btn_punto2X;
	private: System::Windows::Forms::TextBox^ btn_punto2Y;
	private: System::Windows::Forms::TextBox^ btn_punto3X;
	private: System::Windows::Forms::TextBox^ btn_punto3Y;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Button^ btn_Dibujar;
	private: System::Windows::Forms::Button^ btn_Borrar;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::TextBox^ btn_punto4X;
	private: System::Windows::Forms::TextBox^ btn_punto4Y;
	private: System::Windows::Forms::ComboBox^ cajaFormas;
	private: System::Windows::Forms::TextBox^ caja_Grados;
	private: System::Windows::Forms::TextBox^ grado_ubicacion1;
	private: System::Windows::Forms::Button^ btn_rotar;
	private: System::Windows::Forms::TextBox^ grado_ubicacion2;


	private:
		/// <summary>
		/// Variable del diseñador necesaria.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Metodo necesario para admitir el Diseñador. No se puede modificar
		/// el contenido de este metodo con el editor de codigo.
		/// </summary>
		void InitializeComponent(void)
		{
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->btn_demo = (gcnew System::Windows::Forms::Button());
			this->btn_punto1X = (gcnew System::Windows::Forms::TextBox());
			this->btn_punto1Y = (gcnew System::Windows::Forms::TextBox());
			this->btn_punto2X = (gcnew System::Windows::Forms::TextBox());
			this->btn_punto2Y = (gcnew System::Windows::Forms::TextBox());
			this->btn_punto3X = (gcnew System::Windows::Forms::TextBox());
			this->btn_punto3Y = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->btn_Dibujar = (gcnew System::Windows::Forms::Button());
			this->btn_Borrar = (gcnew System::Windows::Forms::Button());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->btn_punto4X = (gcnew System::Windows::Forms::TextBox());
			this->btn_punto4Y = (gcnew System::Windows::Forms::TextBox());
			this->cajaFormas = (gcnew System::Windows::Forms::ComboBox());
			this->caja_Grados = (gcnew System::Windows::Forms::TextBox());
			this->grado_ubicacion1 = (gcnew System::Windows::Forms::TextBox());
			this->btn_rotar = (gcnew System::Windows::Forms::Button());
			this->grado_ubicacion2 = (gcnew System::Windows::Forms::TextBox());
			this->btn_ejeX = (gcnew System::Windows::Forms::Button());
			this->btn_ejeY = (gcnew System::Windows::Forms::Button());
			this->btn_ejeZ = (gcnew System::Windows::Forms::Button());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->CambiarColor = (gcnew System::Windows::Forms::Button());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->label20 = (gcnew System::Windows::Forms::Label());
			this->label21 = (gcnew System::Windows::Forms::Label());
			this->label22 = (gcnew System::Windows::Forms::Label());
			this->txtVEscala = (gcnew System::Windows::Forms::TextBox());
			this->txtXEscala = (gcnew System::Windows::Forms::TextBox());
			this->txtYEscala = (gcnew System::Windows::Forms::TextBox());
			this->btnEscalar = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(25, 20);
			this->pictureBox1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(800, 500);
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			// 
			// btn_demo
			// 
			this->btn_demo->Location = System::Drawing::Point(650, 700);
			this->btn_demo->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->btn_demo->Name = L"btn_demo";
			this->btn_demo->Size = System::Drawing::Size(150, 40);
			this->btn_demo->TabIndex = 1;
			this->btn_demo->Text = L"Demo";
			this->btn_demo->UseVisualStyleBackColor = true;
			this->btn_demo->Click += gcnew System::EventHandler(this, &MyForm::btn_demo_Click);
			// 
			// btn_punto1X
			// 
			this->btn_punto1X->Location = System::Drawing::Point(135, 600);
			this->btn_punto1X->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->btn_punto1X->Name = L"btn_punto1X";
			this->btn_punto1X->Size = System::Drawing::Size(120, 28);
			this->btn_punto1X->TabIndex = 2;
			// 
			// btn_punto1Y
			// 
			this->btn_punto1Y->Location = System::Drawing::Point(295, 600);
			this->btn_punto1Y->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->btn_punto1Y->Name = L"btn_punto1Y";
			this->btn_punto1Y->Size = System::Drawing::Size(120, 28);
			this->btn_punto1Y->TabIndex = 3;
			// 
			// btn_punto2X
			// 
			this->btn_punto2X->Location = System::Drawing::Point(135, 655);
			this->btn_punto2X->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->btn_punto2X->Name = L"btn_punto2X";
			this->btn_punto2X->Size = System::Drawing::Size(120, 28);
			this->btn_punto2X->TabIndex = 4;
			// 
			// btn_punto2Y
			// 
			this->btn_punto2Y->Location = System::Drawing::Point(295, 655);
			this->btn_punto2Y->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->btn_punto2Y->Name = L"btn_punto2Y";
			this->btn_punto2Y->Size = System::Drawing::Size(120, 28);
			this->btn_punto2Y->TabIndex = 5;
			// 
			// btn_punto3X
			// 
			this->btn_punto3X->Location = System::Drawing::Point(135, 710);
			this->btn_punto3X->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->btn_punto3X->Name = L"btn_punto3X";
			this->btn_punto3X->Size = System::Drawing::Size(120, 28);
			this->btn_punto3X->TabIndex = 6;
			// 
			// btn_punto3Y
			// 
			this->btn_punto3Y->Location = System::Drawing::Point(295, 710);
			this->btn_punto3Y->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->btn_punto3Y->Name = L"btn_punto3Y";
			this->btn_punto3Y->Size = System::Drawing::Size(120, 28);
			this->btn_punto3Y->TabIndex = 7;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(30, 603);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(51, 16);
			this->label1->TabIndex = 8;
			this->label1->Text = L"Punto 1:";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(30, 658);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(51, 16);
			this->label2->TabIndex = 9;
			this->label2->Text = L"Punto 2:";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(30, 713);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(51, 16);
			this->label3->TabIndex = 10;
			this->label3->Text = L"Punto 3:";
			// 
			// btn_Dibujar
			// 
			this->btn_Dibujar->Location = System::Drawing::Point(650, 600);
			this->btn_Dibujar->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->btn_Dibujar->Name = L"btn_Dibujar";
			this->btn_Dibujar->Size = System::Drawing::Size(150, 40);
			this->btn_Dibujar->TabIndex = 11;
			this->btn_Dibujar->Text = L"Dibujar";
			this->btn_Dibujar->UseVisualStyleBackColor = true;
			this->btn_Dibujar->Click += gcnew System::EventHandler(this, &MyForm::btn_Dibujar_Click);
			// 
			// btn_Borrar
			// 
			this->btn_Borrar->Location = System::Drawing::Point(650, 650);
			this->btn_Borrar->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->btn_Borrar->Name = L"btn_Borrar";
			this->btn_Borrar->Size = System::Drawing::Size(150, 40);
			this->btn_Borrar->TabIndex = 12;
			this->btn_Borrar->Text = L"Borrar Todo";
			this->btn_Borrar->UseVisualStyleBackColor = true;
			this->btn_Borrar->Click += gcnew System::EventHandler(this, &MyForm::btn_Borrar_Click);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(30, 768);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(51, 16);
			this->label4->TabIndex = 13;
			this->label4->Text = L"Punto 4:";
			// 
			// btn_punto4X
			// 
			this->btn_punto4X->Location = System::Drawing::Point(135, 765);
			this->btn_punto4X->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->btn_punto4X->Name = L"btn_punto4X";
			this->btn_punto4X->Size = System::Drawing::Size(120, 28);
			this->btn_punto4X->TabIndex = 14;
			// 
			// btn_punto4Y
			// 
			this->btn_punto4Y->Location = System::Drawing::Point(295, 765);
			this->btn_punto4Y->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->btn_punto4Y->Name = L"btn_punto4Y";
			this->btn_punto4Y->Size = System::Drawing::Size(120, 28);
			this->btn_punto4Y->TabIndex = 15;
			// 
			// cajaFormas
			//
			this->cajaFormas->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cajaFormas->FormattingEnabled = true;
			this->cajaFormas->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"Triangulo", L"Cuadrilatero" });
			this->cajaFormas->Location = System::Drawing::Point(180, 543);
			this->cajaFormas->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->cajaFormas->Name = L"cajaFormas";
			this->cajaFormas->Size = System::Drawing::Size(200, 24);
			this->cajaFormas->TabIndex = 16;
			this->cajaFormas->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::cajaFormas_SelectedIndexChanged);
			// 
			// caja_Grados
			// 
			this->caja_Grados->Location = System::Drawing::Point(90, 848);
			this->caja_Grados->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->caja_Grados->Name = L"caja_Grados";
			this->caja_Grados->Size = System::Drawing::Size(80, 22);
			this->caja_Grados->TabIndex = 17;
			// 
			// grado_ubicacion1
			// 
			this->grado_ubicacion1->Location = System::Drawing::Point(275, 848);
			this->grado_ubicacion1->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->grado_ubicacion1->Name = L"grado_ubicacion1";
			this->grado_ubicacion1->Size = System::Drawing::Size(80, 22);
			this->grado_ubicacion1->TabIndex = 18;
			// 
			// btn_rotar
			// 
			this->btn_rotar->Location = System::Drawing::Point(570, 845);
			this->btn_rotar->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->btn_rotar->Name = L"btn_rotar";
			this->btn_rotar->Size = System::Drawing::Size(110, 38);
			this->btn_rotar->TabIndex = 19;
			this->btn_rotar->Text = L"Rotar";
			this->btn_rotar->UseVisualStyleBackColor = true;
			this->btn_rotar->Click += gcnew System::EventHandler(this, &MyForm::btn_rotar_Click);
			// 
			// grado_ubicacion2
			// 
			this->grado_ubicacion2->Location = System::Drawing::Point(460, 848);
			this->grado_ubicacion2->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->grado_ubicacion2->Name = L"grado_ubicacion2";
			this->grado_ubicacion2->Size = System::Drawing::Size(80, 22);
			this->grado_ubicacion2->TabIndex = 20;
			// 
			// btn_ejeX
			// 
			this->btn_ejeX->Location = System::Drawing::Point(470, 625);
			this->btn_ejeX->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->btn_ejeX->Name = L"btn_ejeX";
			this->btn_ejeX->Size = System::Drawing::Size(150, 40);
			this->btn_ejeX->TabIndex = 21;
			this->btn_ejeX->Text = L"En eje X";
			this->btn_ejeX->UseVisualStyleBackColor = true;
			this->btn_ejeX->Click += gcnew System::EventHandler(this, &MyForm::btn_ejeX_Click);
			// 
			// btn_ejeY
			// 
			this->btn_ejeY->Location = System::Drawing::Point(470, 675);
			this->btn_ejeY->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->btn_ejeY->Name = L"btn_ejeY";
			this->btn_ejeY->Size = System::Drawing::Size(150, 40);
			this->btn_ejeY->TabIndex = 22;
			this->btn_ejeY->Text = L"En eje Y";
			this->btn_ejeY->UseVisualStyleBackColor = true;
			this->btn_ejeY->Click += gcnew System::EventHandler(this, &MyForm::btn_ejeY_Click);
			// 
			// btn_ejeZ
			// 
			this->btn_ejeZ->Location = System::Drawing::Point(470, 725);
			this->btn_ejeZ->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->btn_ejeZ->Name = L"btn_ejeZ";
			this->btn_ejeZ->Size = System::Drawing::Size(150, 40);
			this->btn_ejeZ->TabIndex = 23;
			this->btn_ejeZ->Text = L"Origen";
			this->btn_ejeZ->UseVisualStyleBackColor = true;
			this->btn_ejeZ->Click += gcnew System::EventHandler(this, &MyForm::btn_ejeZ_Click);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(30, 548);
			this->label5->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(98, 16);
			this->label5->TabIndex = 24;
			this->label5->Text = L"Tipo de Figura:";
			this->label5->Click += gcnew System::EventHandler(this, &MyForm::label5_Click);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(30, 815);
			this->label6->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(77, 16);
			this->label6->TabIndex = 25;
			this->label6->Text = L"ROTACION";
			this->label6->Click += gcnew System::EventHandler(this, &MyForm::label6_Click);
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(470, 595);
			this->label7->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(99, 16);
			this->label7->TabIndex = 26;
			this->label7->Text = L"REFLEXIONES";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(380, 850);
			this->label8->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(61, 16);
			this->label8->TabIndex = 27;
			this->label8->Text = L"Centro Y:";
			this->label8->Click += gcnew System::EventHandler(this, &MyForm::label8_Click);
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(110, 603);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(15, 16);
			this->label9->TabIndex = 28;
			this->label9->Text = L"X:";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(270, 603);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(16, 16);
			this->label10->TabIndex = 29;
			this->label10->Text = L"Y:";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(270, 658);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(16, 16);
			this->label11->TabIndex = 31;
			this->label11->Text = L"Y:";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(110, 658);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(15, 16);
			this->label12->TabIndex = 30;
			this->label12->Text = L"X:";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(270, 713);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(16, 16);
			this->label13->TabIndex = 33;
			this->label13->Text = L"Y:";
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(110, 713);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(15, 16);
			this->label14->TabIndex = 32;
			this->label14->Text = L"X:";
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(110, 768);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(16, 16);
			this->label15->TabIndex = 35;
			this->label15->Text = L"X:";
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Location = System::Drawing::Point(270, 768);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(15, 16);
			this->label16->TabIndex = 34;
			this->label16->Text = L"Y:";
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Location = System::Drawing::Point(30, 850);
			this->label17->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(55, 16);
			this->label17->TabIndex = 36;
			this->label17->Text = L"Grados:";
			// 
			// label18
			// 
			this->label18->AutoSize = true;
			this->label18->Location = System::Drawing::Point(195, 850);
			this->label18->Margin = System::Windows::Forms::Padding(4, 0, 4, 0);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(60, 16);
			this->label18->TabIndex = 37;
			this->label18->Text = L"Centro X:";
			// 
			// CambiarColor
			// 
			this->CambiarColor->Location = System::Drawing::Point(650, 750);
			this->CambiarColor->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->CambiarColor->Name = L"CambiarColor";
			this->CambiarColor->Size = System::Drawing::Size(150, 40);
			this->CambiarColor->TabIndex = 38;
			this->CambiarColor->Text = L"Cambiar color";
			this->CambiarColor->UseVisualStyleBackColor = true;
			this->CambiarColor->Click += gcnew System::EventHandler(this, &MyForm::CambiarColor_Click);
			// 
			// label19
			// 
			this->label19->AutoSize = true;
			this->label19->Location = System::Drawing::Point(30, 900);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(149, 16);
			this->label19->TabIndex = 39;
			this->label19->Text = L"HOMOTECIA / ESCALA";
			// 
			// label20
			// 
			this->label20->AutoSize = true;
			this->label20->Location = System::Drawing::Point(185, 935);
			this->label20->Name = L"label20";
			this->label20->Size = System::Drawing::Size(60, 16);
			this->label20->TabIndex = 40;
			this->label20->Text = L"Centro X:";
			// 
			// label21
			// 
			this->label21->AutoSize = true;
			this->label21->Location = System::Drawing::Point(360, 935);
			this->label21->Name = L"label21";
			this->label21->Size = System::Drawing::Size(61, 16);
			this->label21->TabIndex = 41;
			this->label21->Text = L"Centro Y:";
			// 
			// label22
			// 
			this->label22->AutoSize = true;
			this->label22->Location = System::Drawing::Point(30, 935);
			this->label22->Name = L"label22";
			this->label22->Size = System::Drawing::Size(48, 16);
			this->label22->TabIndex = 42;
			this->label22->Text = L"Factor:";
			// 
			// txtVEscala
			// 
			this->txtVEscala->Location = System::Drawing::Point(90, 933);
			this->txtVEscala->Name = L"txtVEscala";
			this->txtVEscala->Size = System::Drawing::Size(70, 22);
			this->txtVEscala->TabIndex = 43;
			// 
			// txtXEscala
			// 
			this->txtXEscala->Location = System::Drawing::Point(265, 933);
			this->txtXEscala->Name = L"txtXEscala";
			this->txtXEscala->Size = System::Drawing::Size(70, 22);
			this->txtXEscala->TabIndex = 44;
			// 
			// txtYEscala
			// 
			this->txtYEscala->Location = System::Drawing::Point(440, 933);
			this->txtYEscala->Name = L"txtYEscala";
			this->txtYEscala->Size = System::Drawing::Size(70, 22);
			this->txtYEscala->TabIndex = 45;
			// 
			// btnEscalar
			// 
			this->btnEscalar->Location = System::Drawing::Point(535, 930);
			this->btnEscalar->Name = L"btnEscalar";
			this->btnEscalar->Size = System::Drawing::Size(130, 38);
			this->btnEscalar->TabIndex = 46;
			this->btnEscalar->Text = L"Escalar";
			this->btnEscalar->UseVisualStyleBackColor = true;
			this->btnEscalar->Click += gcnew System::EventHandler(this, &MyForm::btnEscalar_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(850, 1000);
			this->Controls->Add(this->btnEscalar);
			this->Controls->Add(this->txtYEscala);
			this->Controls->Add(this->txtXEscala);
			this->Controls->Add(this->txtVEscala);
			this->Controls->Add(this->label22);
			this->Controls->Add(this->label21);
			this->Controls->Add(this->label20);
			this->Controls->Add(this->label19);
			this->Controls->Add(this->CambiarColor);
			this->Controls->Add(this->label18);
			this->Controls->Add(this->label17);
			this->Controls->Add(this->label15);
			this->Controls->Add(this->label16);
			this->Controls->Add(this->label13);
			this->Controls->Add(this->label14);
			this->Controls->Add(this->label11);
			this->Controls->Add(this->label12);
			this->Controls->Add(this->label10);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->btn_ejeZ);
			this->Controls->Add(this->btn_ejeY);
			this->Controls->Add(this->btn_ejeX);
			this->Controls->Add(this->grado_ubicacion2);
			this->Controls->Add(this->btn_rotar);
			this->Controls->Add(this->grado_ubicacion1);
			this->Controls->Add(this->caja_Grados);
			this->Controls->Add(this->cajaFormas);
			this->Controls->Add(this->btn_punto4Y);
			this->Controls->Add(this->btn_punto4X);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->btn_Borrar);
			this->Controls->Add(this->btn_Dibujar);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->btn_punto3Y);
			this->Controls->Add(this->btn_punto3X);
			this->Controls->Add(this->btn_punto2Y);
			this->Controls->Add(this->btn_punto2X);
			this->Controls->Add(this->btn_punto1Y);
			this->Controls->Add(this->btn_punto1X);
			this->Controls->Add(this->btn_demo);
			this->Controls->Add(this->pictureBox1);
			this->Margin = System::Windows::Forms::Padding(3, 2, 3, 2);
			this->Name = L"MyForm";
			this->Text = L"Transformaciones Lineales";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		void DibujarFigura(Graphics^ g, int cx, int cy, array<Point>^ puntosCartesianas) {
			Pen^ figura = gcnew Pen(colorFigura, 2); //Color almacenado
			SolidBrush^ b = gcnew SolidBrush(colorFigura);

			// Convertir coordenadas cartesianas a coordenadas de pantalla
			array<Point>^ puntosPantalla = gcnew array<Point>(puntosCartesianas->Length);
			for (int i = 0; i < puntosCartesianas->Length; i++) {
				int x = cx + puntosCartesianas[i].X;
				int y = cy - puntosCartesianas[i].Y; // Invertimos eje Y
				puntosPantalla[i] = Point(x, y);
			}

			// Dibujar poligono
			g->DrawPolygon(figura, puntosPantalla);

			// Dibujar vertices
			for each (Point p in puntosPantalla) {
				g->FillEllipse(b, p.X - 3, p.Y - 3, 6, 6);
			}
		}


// ═══════════════════════════════════════════════════════════════
// MÉTODOS DE ESTILOS MODERNOS
// ═══════════════════════════════════════════════════════════════

private: void AplicarEstilosModernos()
{
	// COLORES MODERNOS
	Color fondoClaro = Color::FromArgb(248, 249, 250);
	Color colorPrincipal = Color::FromArgb(0, 123, 255);
	
	// FONDO DEL FORMULARIO
	this->BackColor = fondoClaro;
	
	// ESTILO DEL CANVAS
	pictureBox1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
	pictureBox1->BackColor = Color::White;
	
	// BOTONES PRINCIPALES
	btn_Dibujar->FlatStyle = FlatStyle::Flat;
	btn_Dibujar->BackColor = colorPrincipal;
	btn_Dibujar->ForeColor = Color::White;
	btn_Dibujar->FlatAppearance->BorderSize = 0;
	btn_Dibujar->Font = gcnew System::Drawing::Font(L"Segoe UI", 10, FontStyle::Bold);
	btn_Dibujar->Cursor = System::Windows::Forms::Cursors::Hand;
	
	btn_Borrar->FlatStyle = FlatStyle::Flat;
	btn_Borrar->BackColor = Color::FromArgb(220, 53, 69);
	btn_Borrar->ForeColor = Color::White;
	btn_Borrar->FlatAppearance->BorderSize = 0;
	btn_Borrar->Font = gcnew System::Drawing::Font(L"Segoe UI", 10, FontStyle::Bold);
	btn_Borrar->Cursor = System::Windows::Forms::Cursors::Hand;
	
	btn_demo->FlatStyle = FlatStyle::Flat;
	btn_demo->BackColor = Color::FromArgb(40, 167, 69);
	btn_demo->ForeColor = Color::White;
	btn_demo->FlatAppearance->BorderSize = 0;
	btn_demo->Font = gcnew System::Drawing::Font(L"Segoe UI", 10, FontStyle::Bold);
	btn_demo->Cursor = System::Windows::Forms::Cursors::Hand;
	
	// BOTONES DE TRANSFORMACIÓN
	EstilizarBotonSecundario(btn_rotar);
	EstilizarBotonSecundario(btn_ejeX);
	EstilizarBotonSecundario(btn_ejeY);
	EstilizarBotonSecundario(btn_ejeZ);
	EstilizarBotonSecundario(btnEscalar);
	EstilizarBotonSecundario(CambiarColor);
	
	// CAMPOS DE TEXTO
	EstilizarCampoTexto(btn_punto1X);
	EstilizarCampoTexto(btn_punto1Y);
	EstilizarCampoTexto(btn_punto2X);
	EstilizarCampoTexto(btn_punto2Y);
	EstilizarCampoTexto(btn_punto3X);
	EstilizarCampoTexto(btn_punto3Y);
	EstilizarCampoTexto(btn_punto4X);
	EstilizarCampoTexto(btn_punto4Y);
	EstilizarCampoTexto(caja_Grados);
	EstilizarCampoTexto(grado_ubicacion1);
	EstilizarCampoTexto(grado_ubicacion2);
	EstilizarCampoTexto(txtVEscala);
	EstilizarCampoTexto(txtXEscala);
	EstilizarCampoTexto(txtYEscala);
	
	// SELECTOR
	cajaFormas->FlatStyle = FlatStyle::Flat;
	cajaFormas->BackColor = Color::White;
	cajaFormas->Font = gcnew System::Drawing::Font(L"Segoe UI", 10);
	
	// LABELS DE TÍTULOS
	label5->Font = gcnew System::Drawing::Font(L"Segoe UI", 11, FontStyle::Bold);
	label5->BackColor = Color::Transparent;
	label6->Font = gcnew System::Drawing::Font(L"Segoe UI", 10, FontStyle::Bold);
	label6->BackColor = Color::Transparent;
	label7->Font = gcnew System::Drawing::Font(L"Segoe UI", 10, FontStyle::Bold);
	label7->BackColor = Color::Transparent;
	label19->Font = gcnew System::Drawing::Font(L"Segoe UI", 10, FontStyle::Bold);
	label19->BackColor = Color::Transparent;
}

private: void EstilizarBotonSecundario(Button^ boton)
{
	boton->FlatStyle = FlatStyle::Flat;
	boton->BackColor = Color::FromArgb(108, 117, 125);
	boton->ForeColor = Color::White;
	boton->FlatAppearance->BorderSize = 0;
	boton->Font = gcnew System::Drawing::Font(L"Segoe UI", 9, FontStyle::Bold);
	boton->Cursor = System::Windows::Forms::Cursors::Hand;
}

private: void EstilizarCampoTexto(TextBox^ campo)
{
	campo->BorderStyle = BorderStyle::FixedSingle;
	campo->BackColor = Color::White;
	campo->Font = gcnew System::Drawing::Font(L"Segoe UI", 10);
}

// ═══════════════════════════════════════════════════════════════


	private: System::Void btn_demo_Click(System::Object^ sender, System::EventArgs^ e) {
		Graphics^ g = pictureBox1->CreateGraphics();
		Pen^ eje = gcnew Pen(Color::Black, 2);
		Pen^ figura = gcnew Pen(colorFigura, 2);

		// Limpiar el area
		g->Clear(Color::White);

		// Dibujar ejes (centrados en el medio del pictureBox)
		int cx = pictureBox1->Width / 2;
		int cy = pictureBox1->Height / 2;

		g->DrawLine(eje, 0, cy, pictureBox1->Width, cy); // Eje X
		g->DrawLine(eje, cx, 0, cx, pictureBox1->Height); // Eje Y

		// Dibujar un triangulo de demostracion
		// Definir puntos en el sistema de coordenadas logico
		array<Point>^ puntosDemo = {
			Point(50, 80),    // Punto A
			Point(100, 30),  // Punto B
			Point(20, 20)    // Punto C
		};

		// Guardar la figura demo en figuraOriginal para que las transformaciones funcionen
		figuraOriginal = puntosDemo;

		// Dibujar la figura usando el metodo estandar
		DibujarFigura(g, cx, cy, puntosDemo);

		// Habilitar las transformaciones para la figura demo
		btn_rotar->Enabled = true;
		btn_ejeX->Enabled = true;
		btn_ejeY->Enabled = true;
		btn_ejeZ->Enabled = true;
		CambiarColor->Enabled = true;
		caja_Grados->Enabled = true;
		caja_Grados->BackColor = Color::White;
		grado_ubicacion1->Enabled = true;
		grado_ubicacion1->BackColor = Color::White;
		grado_ubicacion2->Enabled = true;
		grado_ubicacion2->BackColor = Color::White;
		txtVEscala->Enabled = true;
		txtVEscala->BackColor = Color::White;
		txtXEscala->Enabled = true;
		txtXEscala->BackColor = Color::White;
		txtYEscala->Enabled = true;
		txtYEscala->BackColor = Color::White;
		btnEscalar->Enabled = true;

		// Actualizar el selector de formas para reflejar que es un triangulo
		cajaFormas->SelectedIndex = 0; // Asumiendo que triangulo es el indice 0

		// Llenar los campos de texto con las coordenadas de la demo (opcional, para que el usuario vea los valores)
		btn_punto1X->Text = "50";
		btn_punto1Y->Text = "80";
		btn_punto2X->Text = "100";
		btn_punto2Y->Text = "30";
		btn_punto3X->Text = "20";
		btn_punto3Y->Text = "20";
	}
	private: System::Void btn_Dibujar_Click(System::Object^ sender, System::EventArgs^ e) {
		if (!ValidarCamposRequeridos()) {
			return;
		}

		try {
			// Crear el grafico y limpiar
			Graphics^ g = pictureBox1->CreateGraphics();
			g->Clear(Color::White);

			// Calcular el centro del pictureBox
			int cx = pictureBox1->Width / 2;
			int cy = pictureBox1->Height / 2;

			// Dibujar ejes
			Pen^ eje = gcnew Pen(Color::Black, 1);
			g->DrawLine(eje, 0, cy, pictureBox1->Width, cy);  // Eje X
			g->DrawLine(eje, cx, 0, cx, pictureBox1->Height); // Eje Y

			// Leer coordenadas (conversion a enteros)
			int Ax = Convert::ToInt32(btn_punto1X->Text);
			int Ay = Convert::ToInt32(btn_punto1Y->Text);
			int Bx = Convert::ToInt32(btn_punto2X->Text);
			int By = Convert::ToInt32(btn_punto2Y->Text);
			int Cx = Convert::ToInt32(btn_punto3X->Text);
			int Cy = Convert::ToInt32(btn_punto3Y->Text);

			// Verificar seleccion
			if (cajaFormas->SelectedItem->ToString() == "Triangulo") {
				// Definir arreglo de 3 puntos
				array<Point>^ triangulo = {
					Point(Ax, Ay),
					Point(Bx, By),
					Point(Cx, Cy)
				};
				// Ordenar puntos para formar un poligono correcto
				triangulo = OrdenarPuntosPoligono(triangulo);
				figuraOriginal = triangulo;
				// Dibujar el triangulo
				DibujarFigura(g, cx, cy, triangulo);
			}
			else if (cajaFormas->SelectedItem->ToString() == "Cuadrilatero") {
				// Leer tambien punto D
				int Dx = Convert::ToInt32(btn_punto4X->Text);
				int Dy = Convert::ToInt32(btn_punto4Y->Text);

				// Definir arreglo de 4 puntos
				array<Point>^ cuadrilatero = {
					Point(Ax, Ay),
					Point(Bx, By),
					Point(Cx, Cy),
					Point(Dx, Dy)
				};
				// Ordenar puntos para formar un poligono correcto
				cuadrilatero = OrdenarPuntosPoligono(cuadrilatero);
				figuraOriginal = cuadrilatero;
				// Dibujar el cuadrilatero
				DibujarFigura(g, cx, cy, cuadrilatero);
			}
			// Habilitar botones de transformacion despues de dibujar exitosamente
			btn_rotar->Enabled = true; // Rotar
			btn_ejeX->Enabled = true; // Espejo X
			btn_ejeY->Enabled = true; // Espejo Y
			btn_ejeZ->Enabled = true; // Espejo Z
			CambiarColor->Enabled = true; // Cambiar color

			// Habilitar campos de rotacion
		caja_Grados->BackColor = Color::White;

		grado_ubicacion1->BackColor = Color::White;

		grado_ubicacion2->BackColor = Color::White;

			caja_Grados->Enabled = true;
			grado_ubicacion1->Enabled = true;
		txtVEscala->BackColor = Color::White;

		txtXEscala->BackColor = Color::White;

		txtYEscala->BackColor = Color::White;

			grado_ubicacion2->Enabled = true;

			// Habilitar campos y boton de homotecia/escala
			txtVEscala->Enabled = true;
			txtXEscala->Enabled = true;
			txtYEscala->Enabled = true;
			btnEscalar->Enabled = true;
		}
		catch (Exception^ ex) {
			MessageBox::Show("Error: Verifique que todas las coordenadas sean numeros validos.", "Error de entrada", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
		

	}
private: System::Void btn_Borrar_Click(System::Object^ sender, System::EventArgs^ e) {
	Graphics^ g = pictureBox1->CreateGraphics();
	Pen^ eje = gcnew Pen(Color::Black, 2);
	g->Clear(Color::White);
	int cx = pictureBox1->Width / 2;
	int cy = pictureBox1->Height / 2;

	g->DrawLine(eje, 0, cy, pictureBox1->Width, cy); // Eje X
	g->DrawLine(eje, cx, 0, cx, pictureBox1->Height); // Eje Y

	// Limpiar la figura original
	figuraOriginal = nullptr;

	// NO limpiar los campos de puntos - mantener los valores ingresados
	// para poder redibujar sin tener que ingresarlos nuevamente

	// Limpiar solo los campos de transformaciones (opcionales)
	caja_Grados->Text = "";
	grado_ubicacion1->Text = "";
	grado_ubicacion2->Text = "";
	txtVEscala->Text = "";
	txtXEscala->Text = "";
	txtYEscala->Text = "";

	// NO deshabilitar campos de coordenadas - mantenerlos habilitados
	// para poder redibujar inmediatamente

	// Deshabilitar campos de rotacion (hasta que se dibuje nuevamente)
	caja_Grados->Enabled = false;
	grado_ubicacion1->Enabled = false;
	grado_ubicacion2->Enabled = false;

	// Deshabilitar campos de homotecia (hasta que se dibuje nuevamente)
	txtVEscala->Enabled = false;
	txtXEscala->Enabled = false;
	txtYEscala->Enabled = false;

	// Deshabilitar botones de transformaciones (hasta que se dibuje nuevamente)
	btn_rotar->Enabled = false;
	btn_ejeX->Enabled = false;
	btn_ejeY->Enabled = false;
	btn_ejeZ->Enabled = false;
	CambiarColor->Enabled = false;
	btnEscalar->Enabled = false;

	// Mantener habilitado el boton Dibujar para poder redibujar
	// btn_Dibujar sigue habilitado si hay una forma seleccionada
}
private: System::Void cajaFormas_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
	if (cajaFormas->SelectedIndex != -1)
	{
		// Habilitar campos basicos de coordenadas
		btn_punto1X->Enabled = true;
		btn_punto1X->BackColor = Color::White;
		btn_punto1Y->Enabled = true;
		btn_punto1Y->BackColor = Color::White;
		btn_punto2X->Enabled = true;
		btn_punto2X->BackColor = Color::White;
		btn_punto2Y->Enabled = true;
		btn_punto2Y->BackColor = Color::White;
		btn_punto3X->Enabled = true;
		btn_punto3X->BackColor = Color::White;
		btn_punto3Y->Enabled = true;
		btn_punto3Y->BackColor = Color::White;

		// NO habilitar campos de rotacion aqui
		// Se habilitaran solo despues de dibujar la figura
		// caja_Grados->Enabled = true;
		// grado_ubicacion1->Enabled = true;
		// grado_ubicacion2->Enabled = true;

		// Habilitar boton de dibujar
		btn_Dibujar->Enabled = true;

		if ( cajaFormas->SelectedItem->ToString() == "Triangulo") {
			btn_punto4X->Enabled = false;
			btn_punto4X->BackColor = Color::FromArgb(240, 240, 240);
			btn_punto4Y->Enabled = false;
			btn_punto4Y->BackColor = Color::FromArgb(240, 240, 240);
			// Limpiar los campos del cuarto punto
			btn_punto4X->Text = "";
			btn_punto4Y->Text = "";
		}
		else if (cajaFormas->SelectedItem->ToString() == "Cuadrilatero") {
			btn_punto4X->Enabled = true;
			btn_punto4X->BackColor = Color::White;
			btn_punto4Y->Enabled = true;
			btn_punto4Y->BackColor = Color::White;
		}
	}
	else
	{
		// Si no hay seleccion, deshabilitar todo

	}
}
private: System::Void btn_rotar_Click(System::Object^ sender, System::EventArgs^ e) {
	// Validar que haya una figura dibujada
	if (figuraOriginal == nullptr) {
		MessageBox::Show("Primero debe dibujar una figura.", "Validacion", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		return;
	}

	// Validar campos de rotacion
	if (!ValidarCamposRotacion()) {
		return;
	}

	Graphics^ g = pictureBox1->CreateGraphics();
	g->Clear(Color::White);

	int cx = pictureBox1->Width / 2;
	int cy = pictureBox1->Height / 2;

	// Dibujar ejes
	Pen^ eje = gcnew Pen(Color::Black, 1);
	g->DrawLine(eje, 0, cy, pictureBox1->Width, cy);
	g->DrawLine(eje, cx, 0, cx, pictureBox1->Height);

	try {
		// Leer angulo y centro de rotacion
		double angulo = Convert::ToDouble(caja_Grados->Text);
		double rad = angulo * Math::PI / 180.0; // convertir a radianes

		double h = Convert::ToDouble(grado_ubicacion1->Text);
		double k = Convert::ToDouble(grado_ubicacion2->Text);

		// Crear lista de puntos transformados
		array<Point>^ figuraRotada = gcnew array<Point>(figuraOriginal->Length);

		for (int i = 0; i < figuraOriginal->Length; i++) {
			double x = figuraOriginal[i].X;
			double y = figuraOriginal[i].Y;

			// Trasladar al centro de rotacion
			double xt = x - h;
			double yt = y - k;

			// Aplicar rotacion
			double xr = xt * Math::Cos(rad) - yt * Math::Sin(rad);
			double yr = xt * Math::Sin(rad) + yt * Math::Cos(rad);

			// Volver a trasladar
			xr += h;
			yr += k;

			figuraRotada[i] = Point((int)xr, (int)yr);
		}

		// Dibujar figura original (azul)
		Pen^ azul = gcnew Pen(Color::Blue, 2);
		DibujarFigura(g, cx, cy, figuraOriginal);

		// Dibujar figura rotada (rojo)
		Pen^ rojo = gcnew Pen(Color::Red, 2);
		DibujarFigura(g, cx, cy, figuraRotada);

	}
	catch (Exception^ ex) {
		MessageBox::Show("Error: Verifique que los valores de rotacion sean numeros validos.", "Error de entrada", MessageBoxButtons::OK, MessageBoxIcon::Error);
	}
}
private: System::Void btn_ejeX_Click(System::Object^ sender, System::EventArgs^ e) {
	if (figuraOriginal == nullptr) {
		MessageBox::Show("Primero debe dibujar una figura.", "Validacion", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		return;
	}

	// Crear arreglo para la figura reflejada
	array<Point>^ figuraReflejada = gcnew array<Point>(figuraOriginal->Length);

	for (int i = 0; i < figuraOriginal->Length; i++) {
		int x = figuraOriginal[i].X;
		int y = -figuraOriginal[i].Y; // reflejo en X
		figuraReflejada[i] = Point(x, y);
	}

	Graphics^ g = pictureBox1->CreateGraphics();
	int cx = pictureBox1->Width / 2;
	int cy = pictureBox1->Height / 2;

	// Dibujar reflejada
	DibujarFigura(g, cx, cy, figuraReflejada);
}
private: System::Void btn_ejeY_Click(System::Object^ sender, System::EventArgs^ e) {
	if (figuraOriginal == nullptr) {
		MessageBox::Show("Primero debe dibujar una figura.", "Validacion", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		return;
	}

	array<Point>^ figuraReflejada = gcnew array<Point>(figuraOriginal->Length);

	for (int i = 0; i < figuraOriginal->Length; i++) {
		int x = -figuraOriginal[i].X; // reflejo en Y
		int y = figuraOriginal[i].Y;
		figuraReflejada[i] = Point(x, y);
	}

	Graphics^ g = pictureBox1->CreateGraphics();
	int cx = pictureBox1->Width / 2;
	int cy = pictureBox1->Height / 2;

	DibujarFigura(g, cx, cy, figuraReflejada);
}
private: System::Void btn_ejeZ_Click(System::Object^ sender, System::EventArgs^ e) {
	if (figuraOriginal == nullptr) {
		MessageBox::Show("Primero debe dibujar una figura.", "Validacion", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		return;
	}

	array<Point>^ figuraReflejada = gcnew array<Point>(figuraOriginal->Length);

	for (int i = 0; i < figuraOriginal->Length; i++) {
		int x = -figuraOriginal[i].X;
		int y = -figuraOriginal[i].Y; // reflejo en origen
		figuraReflejada[i] = Point(x, y);
	}

	Graphics^ g = pictureBox1->CreateGraphics();
	int cx = pictureBox1->Width / 2;
	int cy = pictureBox1->Height / 2;

	DibujarFigura(g, cx, cy, figuraReflejada);
}
private: System::Void label5_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void label8_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void label6_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void button8_Click(System::Object^ sender, System::EventArgs^ e) {
}
private: System::Void CambiarColor_Click(System::Object^ sender, System::EventArgs^ e) {

	// Validar que haya una figura dibujada
	if (figuraOriginal == nullptr) {
		MessageBox::Show("Primero debe dibujar una figura para cambiar su color.", "Validacion", MessageBoxButtons::OK, MessageBoxIcon::Warning);
		return;
	}
	//Crear el dialogo de color
	ColorDialog^ colorDialog = gcnew ColorDialog();
	colorDialog->AnyColor = true;
	colorDialog->FullOpen = true;

	//mostrar dialogo de color
	if (colorDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
		//Cambiar el color de la figura
		colorFigura = colorDialog->Color;

		//Redibujar la figura
		if (figuraOriginal != nullptr) {
			Graphics^ g = pictureBox1->CreateGraphics();
			g->Clear(Color::White);

			int cx = pictureBox1->Width / 2;
			int cy = pictureBox1->Height / 2;

			//Dibujar Ejes
			Pen^ eje = gcnew Pen(Color::Black, 1);
			g->DrawLine(eje, 0, cy, pictureBox1->Width, cy);
			g->DrawLine(eje, cx, 0, cx, pictureBox1->Height);

			//Dibujar la figura con nuevo color
			DibujarFigura(g, cx, cy, figuraOriginal);
		}
	}
}
private: System::Void btnEscalar_Click(System::Object^ sender, System::EventArgs^ e) {
	if (figuraOriginal == nullptr) return;

	try {
		// Leer valores ingresados por el usuario
		double k = Convert::ToDouble(txtVEscala->Text);
		int x0 = Convert::ToInt32(txtXEscala->Text);
		int y0 = Convert::ToInt32(txtYEscala->Text);

		array<Point>^ figuraHomotecia = gcnew array<Point>(figuraOriginal->Length);

		for (int i = 0; i < figuraOriginal->Length; i++) {
			int x = figuraOriginal[i].X;
			int y = figuraOriginal[i].Y;

			int xPrime = x0 + (int)(k * (x - x0));
			int yPrime = y0 + (int)(k * (y - y0));

			figuraHomotecia[i] = Point(xPrime, yPrime);
		}

		Graphics^ g = pictureBox1->CreateGraphics();
		int cx = pictureBox1->Width / 2;
		int cy = pictureBox1->Height / 2;

		// Dibujar figura homotetica
		DibujarFigura(g, cx, cy, figuraHomotecia);

		// --- Dibujar el centro de homotecia ---
		int pxCentro = cx + x0;
		int pyCentro = cy - y0;
		SolidBrush^ centroBrush = gcnew SolidBrush(Color::Black);
		g->FillEllipse(centroBrush, pxCentro - 4, pyCentro - 4, 8, 8);

		// --- Dibujar lineas de proyeccion ---
		Pen^ proyeccion = gcnew Pen(Color::Gray, 1);
		for (int i = 0; i < figuraOriginal->Length; i++) {
			int pxOrig = cx + figuraOriginal[i].X;
			int pyOrig = cy - figuraOriginal[i].Y;

			int pxHomo = cx + figuraHomotecia[i].X;
			int pyHomo = cy - figuraHomotecia[i].Y;

			// (1) linea centro → original
			g->DrawLine(proyeccion, pxCentro, pyCentro, pxOrig, pyOrig);

			// linea desde el centro hasta el homotetico (pasa por el original tambien)
			g->DrawLine(proyeccion, pxCentro, pyCentro, pxHomo, pyHomo);

			// dibujar tambien el punto original en azul clarito (referencia opcional)
			SolidBrush^ refBrush = gcnew SolidBrush(Color::LightBlue);
			g->FillEllipse(refBrush, pxOrig - 3, pyOrig - 3, 6, 6);
		}
	}
	catch (Exception^ ex) {
		MessageBox::Show("Error en los datos de homotecia: " + ex->Message);
	}
}
};
}
