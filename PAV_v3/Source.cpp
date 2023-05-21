#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <conio.h>
#include <fstream>
#include <Windows.h>
#include <Windowsx.h>
#include <Commctrl.h>
#include "resource.h"
#pragma warning(disable : 4996)
using namespace std;

//general
char  ActUserPass[15], ActUser[15], aux[50], F[50], nofoto[MAX_PATH] = "C:\\Users\\giova\\Desktop\\PAV_v3", fotoAux[MAX_PATH],auxvalidacion[50],charAUXFN[50],Comer[20],ComerAux[20];
bool cambioSpace=false,deshabilitar=false, deshabilitarAux=false,cajero=false;
//usuarios
bool Vacio,inicio=true, verificarExistenciaAdmin = false;
int id = 0,contUser=0;
//clientes
int idCliente = 0,contCliente=0;
//promos
int idPromo = 0,activas=0,inactivas=0;
//consumos
bool descuento = false;
int idConsumo = 0,contconsumo=0,descuentoaplicado=0;
float nuevoTotal=0,totalAux = 0;
float GranTotal=0;
char GranTotalC[50];

void fecha() {
	char tt[100];
	time_t now = time(nullptr);
	auto tm_info = localtime(&now);

	strftime(tt, 100, "%Y-%m-%d|-|%H:%M:%S", tm_info);
	strcpy(F, tt);
}
struct Comercio
{
	char comercio[20];
	Comercio* sgteComercio;
}; Comercio* listaComercio=nullptr;
//*************************************************************************
struct user //admin
{
	int ID;
	char UserName[15], password[10],foto[MAX_PATH];
	bool admin;
	char FullnameAdmin[50], aliasAdmin[10];
	user* sgteUser;
};
user* listaUser = nullptr;
//**************************************************************************
struct userCliente
{
	int IDCliente;
	char nombreCliente[50], aliasCliente[15], email[50];
	char telefono[11];
	char Fecha[50];
	char compra[50];
	userCliente* sgteUserCliente;

};
userCliente* listaClientes = nullptr;
//**************************************************************************
struct Promo
{
	int IDPromo;
	char nombrePromo[50], fechaPromo[50];
	float montoPromo, descuentoPromo;
	bool status = false;
	Promo* sgtePromo;
}; Promo* listaPromo = nullptr;
//**************************************************************************

//**************************************************************************
LRESULT CALLBACK fComercios(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK fAlta_Comercios(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK fLogin(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK fLogin_Alta_Admin(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK fPrincipal_Admin(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK fListado_Users(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK fListado_USers_Actualizar(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK factualizarPass(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK fPrincipal_Cajero(HWND, UINT, WPARAM, LPARAM);


void GuardarComercio() {
	ofstream archivo;
	char aux[50];
	
	archivo.open("Comercios.bin", ios::out | ios::trunc | ios::binary);
	Comercio* actual = new Comercio;
	actual = listaComercio;
	while (actual != nullptr)
	{
		archivo << actual->comercio << endl;
		actual = actual->sgteComercio;
	}
	archivo.close();
}
void CargarComercio() {
	int  ant = 0, contleer = 0;
	char info[50];
	Comercio* nuevo = nullptr;
	int idaux;
	ifstream archivo("Comercios.bin", ios::in, ios::binary);
	if (archivo.is_open())
	{
		while (!archivo.eof()) {
			if (contleer == 0)
			{
				nuevo = new Comercio;
			}
			archivo >> info;
			if (strcmp(info, "")!=0)
			{
				switch (contleer) {
				case 0:
					strcpy(nuevo->comercio,info);
					contleer = 0;
					Comercio* aux1 = listaComercio;
					
					if (listaComercio == aux1)
					{
						listaComercio = nuevo;
					}
					nuevo->sgteComercio = aux1;
					strcpy(info, "");
					break;
				}
			}
		}
	}
	archivo.close();
}
void GuardarUser() {
	ofstream archivo;
	char aux[50],name[50];
	strcpy(name, strcat(Comer, "_Users.bin"));
	strcpy(Comer, ComerAux);
	archivo.open(name, ios::out| ios::trunc |ios::binary);
	user* actual = new user;
	actual = listaUser;
	while (actual != nullptr)
	{
		archivo << actual->ID << endl;
		archivo << actual->admin << endl;//0 es cajero,1 es admin
		archivo << actual->UserName << endl;
		archivo << actual->password << endl;
		archivo << actual->foto<< endl;
		archivo << actual->FullnameAdmin << endl;
		archivo << actual->aliasAdmin << endl;
		actual = actual->sgteUser;
	}
	archivo.close();
}
void CargarUser() {
	int  ant = 0, contleer = 0;
	bool contnodo;
	char info[MAX_PATH];
	user* nuevo = nullptr;
	int idaux;
	char name[50];
	strcpy(name, strcat(Comer, "_Users.bin"));
	strcpy(Comer, ComerAux);
	ifstream archivo(name, ios::in| ios::binary);
	if (archivo.is_open())
	{
		while (!archivo.eof()) {
			if (contleer==0)
			{
				nuevo = new user();
			}
			archivo >> info; 
			if (strcmp(info, "") != 0)
			{
				switch (contleer)
				{
				case 0:
					nuevo->ID = atoi(info);
					contleer++;
					break;
				case 1:
					if (strcmp(info,"0")==0)
					{
                      nuevo->admin = false;
					}
					else
					{
						deshabilitar = true;
						deshabilitarAux = true;
						nuevo->admin = true;
					}
					contleer++;
					break;
				case 2:
					strcpy(nuevo->UserName, info);
					contleer++;
					break;
				case 3:
					strcpy(nuevo->password, info);
					
					contleer++;
					break;
				case 4:
					strcpy(nuevo->foto, info);

					contleer++;
					break;
				case 5:
					strcpy(nuevo->FullnameAdmin, info);
					contleer++;
					break;
				case 6:{
					strcpy(nuevo->aliasAdmin, info);
					id = nuevo->ID;
					contUser++;
					idaux = id + 1;
					contleer = 0;
					contnodo = true;
					user* aux1 = listaUser;
					user* aux2 = nullptr;
					while (aux1 != nullptr and aux1->ID <= idaux)
					{
						aux2 = aux1;
						aux1 = aux1->sgteUser;
					}

					if (listaUser == aux1)
					{
						listaUser = nuevo;
					}
					else
					{
						aux2->sgteUser = nuevo;
					}
					nuevo->sgteUser = aux1;
					strcpy(info, "");
					}break;
				default:
					break;
				}
			}
		}
	}

	archivo.close();
	
}
void GuardarCliente() {
	ofstream archivo;
	char aux[50], name[50];
	
	strcpy(name, strcat(Comer, "_Clientes.bin"));
	strcpy(Comer, ComerAux);
	archivo.open(name, ios::out | ios::trunc | ios::binary);
	userCliente* actual = new userCliente;
	actual = listaClientes;
	while (actual != nullptr)
	{
		archivo << actual->IDCliente << endl;
		archivo << actual->nombreCliente << endl;//0 es cajero,1 es admin
		archivo << actual->aliasCliente << endl;
		archivo << actual->email << endl;
		archivo << actual->telefono << endl;
		archivo << actual->Fecha<< endl;
		
		actual = actual->sgteUserCliente;
	}
	archivo.close();
}
void CargarCliente() {
	int  ant = 0, contleer = 0;
	bool contnodo;
	char info[MAX_PATH];
	userCliente* nuevo = nullptr;
	int idaux; 
	char name[50];
	strcpy(name, strcat(Comer, "_Clientes.bin"));
	strcpy(Comer, ComerAux);
	contCliente = 0;
	ifstream archivo(name, ios::in | ios::binary);
	if (archivo.is_open())
	{
		while (!archivo.eof()) {
			if (contleer == 0)
			{
				nuevo = new userCliente();
			}
			archivo >> info;
			if (strcmp(info, "") != 0)
			{
				switch (contleer)
				{
				case 0:
					nuevo->IDCliente = atoi(info);
					contleer++;
					break;
				case 1:
						strcpy(nuevo->nombreCliente,info);
					contleer++;
					break;
				case 2:
					strcpy(nuevo->aliasCliente, info);
					contleer++;
					break;
				case 3:
					strcpy(nuevo->email, info);

					contleer++;
					break;
				case 4:
					strcpy(nuevo->telefono, info);
					contleer++;
					break; 
				case 5: {
					strcpy(nuevo->Fecha, info);
					idCliente=nuevo->IDCliente;
					contCliente++;
					idaux = idCliente + 1;
					contleer = 0;
					contnodo = true;
					userCliente* aux1 = listaClientes;
					userCliente* aux2 = nullptr;
					while (aux1 != nullptr and aux1->IDCliente <= idaux)
					{
						aux2 = aux1;
						aux1 = aux1->sgteUserCliente;
					}

					if (listaClientes == aux1)
					{
						listaClientes = nuevo;
					}
					else
					{
						aux2->sgteUserCliente = nuevo;
					}
					nuevo->sgteUserCliente = aux1;
					strcpy(info, "");
				}break;
				default:
					break;
				}
			}
		}
	}
	archivo.close();

}
void GuardarPromo() {
	ofstream archivo;
	char aux[50], name[50];

	strcpy(name, strcat(Comer, "_Promos.bin"));
	strcpy(Comer, ComerAux);
	archivo.open(name, ios::out | ios::trunc | ios::binary);

	Promo* actual = new Promo;
	actual = listaPromo;
	while (actual != nullptr)
	{
		archivo << actual->IDPromo << endl;
		archivo << actual->nombrePromo << endl;//0 es cajero,1 es admin
		archivo << actual->fechaPromo<< endl;
		archivo << actual->montoPromo << endl;
		archivo << actual->descuentoPromo << endl;
		archivo << actual->status << endl;

		actual = actual->sgtePromo;
	}
	archivo.close();
}
void CargarPromo() {
	int  ant = 0, contleer = 0;
	bool contnodo;
	char info[MAX_PATH];
	Promo* nuevo = nullptr;
	int idaux;
	char name[50];
	int auxCliente = contCliente;
	 
	strcpy(name, strcat(Comer, "_Promos.bin"));
	strcpy(Comer, ComerAux);
     contCliente =auxCliente;
	ifstream archivo(name, ios::in | ios::binary);
	if (archivo.is_open())
	{
		while (!archivo.eof()) {
			if (contleer == 0)
			{
				nuevo = new Promo();
			}
			archivo >> info;
			if (strcmp(info, "") != 0)
			{
				switch (contleer)
				{
				case 0:
					nuevo->IDPromo = atoi(info);
					contleer++;
					break;
				case 1:
					strcpy(nuevo->nombrePromo, info);
					contleer++;
					break;
				case 2:
					strcpy(nuevo->fechaPromo, info);
					contleer++;
					break;
					break;
				case 3:
					nuevo->montoPromo=atof (info);
					contleer++;
					break;
				case 4:
					nuevo->descuentoPromo = atoi(info);

					contleer++;
					break;
				case 5: {
					if (strcmp(info, "0") == 0)
					{
						nuevo->status = false;
						inactivas++;
					}
					else
					{
						nuevo->status = true;
						activas++;
					}
					idPromo = nuevo->IDPromo;
					idaux = idPromo + 1;
					contleer = 0;
					contnodo = true;
					Promo* aux1 = listaPromo;
					Promo* aux2 = nullptr;
					while (aux1 != nullptr and aux1->IDPromo <= idaux)
					{
						aux2 = aux1;
						aux1 = aux1->sgtePromo;
					}

					if (listaPromo == aux1)
					{
						listaPromo = nuevo;
					}
					else
					{
						aux2->sgtePromo = nuevo;
					}
					nuevo->sgtePromo = aux1;
					strcpy(info, "");
				}break;
				default:
					break;
				}
			}
		}
	}
	archivo.close();

}

void GuardarExcel() {
	ofstream archivo;
	char aux[50];
	strcpy(aux, strcat(Comer, "_tablaExcel.csv"));
	strcpy(Comer, ComerAux);
	archivo.open(aux, ios::out | ios::trunc | ios::binary);
	user* actual = new user;
	actual = listaUser;archivo << "usuarios"<<endl;archivo << "ID," << "tipo de usuario," << "nombre de usuario," << "contraseña," << "direccion foto," << "nombre completo," << "alias" << endl;
	while (actual != nullptr)
	{
		
		
		archivo << actual->ID << ",";
		archivo << actual->admin << ",";//0 es cajero,1 es admin
		archivo << actual->UserName << ",";
		archivo << actual->password << ",";
		archivo << actual->foto << ",";
		archivo << actual->FullnameAdmin << ",";
		archivo << actual->aliasAdmin << endl;
		actual = actual->sgteUser;
	}
	userCliente* actual2 = new userCliente;
	actual2 = listaClientes;archivo <<endl<< "Clientes" << endl;archivo << "ID," << "nombre Cliente," << "alias," << "Email," << "telefono," << "fecha" << endl;
	while (actual2 != nullptr)
	{
		
		
		archivo << actual2->IDCliente << ",";
		archivo << actual2->nombreCliente << ",";//0 es cajero,1 es admin
		archivo << actual2->aliasCliente << ",";
		archivo << actual2->email << ",";
		archivo << actual2->telefono << ",";
		archivo << actual2->Fecha << endl;

		actual2 = actual2->sgteUserCliente;
	}
	Promo* actual3 = new Promo;
	actual3 = listaPromo;archivo << endl << "Promociones" << endl;archivo << "ID," << "nombre promo," << "fecha promo," << "monto," << "descuento," << "status" << endl;
	while (actual3 != nullptr)
	{
		
		
		archivo << actual3->IDPromo << ",";
		archivo << actual3->nombrePromo << ",";//0 es cajero,1 es admin
		archivo << actual3->fechaPromo << ",";
		archivo << actual3->montoPromo << ",";
		archivo << actual3->descuentoPromo << ",";
		archivo << actual3->status << endl;

		actual3 = actual3->sgtePromo;
	}
	
	archivo.close();
}
int validarComercio(char variable[]) {
	int i = 0;
	while (variable[i]) {
		// Si no es del alfabeto y no es un espacio regresamos false o 0
		if (!isalnum(variable[i]) && variable[i] != ' ') {
			return 0;
		}
		if (variable[i] == ' ')
		{
			variable[i] = '_';
		}
		i++;
	}
	if (i < 5 or i>15)
	{
		return 0;
	}
	return 1;

}
int sololetras(char variable[]) {
	int i=0;
	while (variable[i]) {
		
		if (!isalpha(variable[i])) {
          return 0;
		}
			
		i++;
	}
	return 1;
}
int validarPass(char variable[]) {
	int i = 0;
	bool letra=false, numero=false, especial=false;
	while (variable[i]) {
		// Si no es del alfabeto y no es un espacio regresamos false o 0
		if (isalpha(variable[i])) {
			letra = true;
		}
		if (isdigit(variable[i]))
		{
			numero = true;
		}
		if (!isalnum(variable[i]))
		{
			especial = true;
		}
		i++;
	}
	if (letra and numero and especial)
	{
      return 1;
	}
	else
	{
		return 0;
	}
	
}
int validarFullName(char variable[]) {
	int i = 0;
	while (variable[i]) {
		// Si no es del alfabeto y no es un espacio regresamos false o 0
		if (!isalpha(variable[i]) && variable[i] != ' ') {
			return 0;
		}
		if (variable[i] == ' ')
		{
			variable[i] = '_';
		}
		i++;
	}
	return 1;

}
int cambiarSpace(char variable[]) {
	int i = 0;
	while (variable[i]) {

		if (variable[i] == '_')
		{
				variable[i] = ' ';
		}
		i++;
	}
	return 1;

}
int cambiaraGuion(char variable[]) {
	int i = 0;
	while (variable[i]) {

		if (variable[i] == ' ')
		{
			variable[i] = '_';
		}
		i++;
	}
	return 1;
}
int validarAlias(char variable[]) {
	int i = 0;
	while (variable[i]) {
		if (!isalnum(variable[i])) {
			return 0;
		}

		i++;
	}
	return 1;
}
int soloNumeros(char variable[]) {
	int i = 0;
	while (variable[i]) {

		if (!isdigit(variable[i])) {
			return 0;
		}

		i++;
	}
	return 1;
}
int soloFloat(char variable[]) {
	int i = 0;
	bool punto = false;
	while (variable[i]) {
		
		if (!isdigit(variable[i]) and variable[i]!='.') {
			return 0;
		}

		i++;
	}
	return 1;
}
int validarEmail(char variable[]) {
	int i = 0;
	bool letraI = false, arroba = false,letraM=false, punto = false,Letraf=false;
	while (variable[i]) {

		if (isalnum(variable[0])) {
			letraI = true;
		}
		if (variable[i] == '@' and letraI)
		{
			arroba = true;
		}
		if (isalpha(variable[i]) and arroba)
		{
			letraM = true;
		}
		if (variable[i]=='.' and letraM)
		{
			punto = true;
		}
		if (variable[i]=='.' and punto) {
			Letraf = true;
		}
		i++;
	}
		if (letraI and arroba and punto and Letraf)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	
}
int validarDuplicadoUser(HWND hwnd) {
	user* actual = new user;
	char userr[50];
	actual = listaUser;
	strcpy(userr, "Ya hay un usuario llamado ");
	while (actual != nullptr)
	{
		if (strcmp(aux, actual->UserName) == 0)
		{
				strcat(userr, actual->UserName);
			MessageBox(hwnd, userr, "ATENCION", MB_OK | MB_ICONERROR);
			return false;
			break;
		}
		actual = actual->sgteUser;
	}
	return true;
}
int validarDuplicadoCliente(HWND hwnd) {
	userCliente* actual = new userCliente;
	char userr[50];
	actual = listaClientes;
	strcpy(userr, "Ya hay un cliente llamado ");
	while (actual != nullptr)
	{

		if (strcmp(aux, actual->nombreCliente) == 0)
		{
			strcat(userr, actual->nombreCliente);
			MessageBox(hwnd, userr, "ATENCION", MB_OK | MB_ICONERROR);
			return false;
			break;
		}
		actual = actual->sgteUserCliente;
	}
	return true;
}
int validarDuplicadoPromo(HWND hwnd) {
	Promo* actual = new Promo;
	char userr[50];
	actual = listaPromo;
	strcpy(userr, "Ya hay una promocion llamada ");
	while (actual != nullptr)
	{
		if (strcmp(aux, actual->nombrePromo) == 0)
		{
			strcat(userr, actual->nombrePromo);
			MessageBox(hwnd, userr, "ATENCION", MB_OK | MB_ICONERROR);
			return false;
			break;
		}
		actual = actual->sgtePromo;
	}
	return true;
}
int WINAPI WinMain(HINSTANCE hinstanciaAct, HINSTANCE hInstanciaPrev, PSTR CmdLine, int CShow) {

	HWND hComercios = CreateDialog(hinstanciaAct, MAKEINTRESOURCE(DLG_Comercio), NULL, fComercios);
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	ShowWindow(hComercios, CShow);

	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}

	return 0;
}
LRESULT CALLBACK fComercios(HWND hwnd, UINT msg, WPARAM wPARAM, LPARAM lPARAM) {

	switch (msg)
	{
	case WM_INITDIALOG: {
		SendMessage(GetDlgItem(hwnd, LB_Comercios), LB_RESETCONTENT, 0, 0);
		if (inicio)
		{
          CargarComercio();
		}
	     
		Comercio* actual = new Comercio;

		actual = listaComercio;
		while (actual != nullptr)
		{
			SendMessage(GetDlgItem(hwnd, LB_Comercios), LB_ADDSTRING, 0, (LPARAM)actual->comercio);
			actual = actual->sgteComercio;
		}
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wPARAM))
		{
		case BTN_Comercio_Aceptar: {
			strcpy(Comer, "");
			int lb = SendMessage(GetDlgItem(hwnd, LB_Comercios), LB_GETCURSEL, 0, 0);
			SendMessage(GetDlgItem(hwnd, LB_Comercios), LB_GETTEXT, lb, (LPARAM)Comer);
			if (strcmp(Comer,"")==0)
			{
				MessageBox(hwnd, "Seleccione un comercio", "ATENCION", MB_OK | MB_ICONERROR);
				break;
			}
			strcpy(ComerAux, Comer);
			EndDialog(hwnd, wPARAM);
			inicio = true;
			DialogBox(NULL, MAKEINTRESOURCE(DLG_Login), hwnd, fLogin);

		}break;
		case BTN_Comercios_Registrar: {
			int lb = SendMessage(GetDlgItem(hwnd, LB_Comercios), LB_GETCURSEL, 0, 0);
			SendMessage(GetDlgItem(hwnd, LB_Comercios), LB_GETTEXT, lb, (LPARAM)aux);
			EndDialog(hwnd, wPARAM);
			DialogBox(NULL, MAKEINTRESOURCE(DLG_Alta_Comercio), hwnd, fAlta_Comercios);
		}break;
		case BTN_Comercios_Salir:
			SendMessage(hwnd, WM_DESTROY, 0, 0);
			break;
		}
	}

				   break;
	case WM_DESTROY:
		PostQuitMessage(0);
	}
	return 0;
}
LRESULT CALLBACK fAlta_Comercios(HWND hwnd, UINT msg, WPARAM wPARAM, LPARAM lPARAM) {
	switch (msg)
	{
	case WM_INITDIALOG: {
		inicio = false;
	}
	case WM_COMMAND: {
		switch (LOWORD(wPARAM))
		{
		case BTN_Alta_Comercio_Guardar: {
			Comercio* nuevo = new Comercio();
			GetDlgItemText(hwnd, EC_Alta_Comercio_Name, nuevo->comercio, 20);
			if(!validarComercio(nuevo->comercio)) {
				MessageBox(hwnd, "El comercio solo puede contener de 5 a 15 entre letras,numeros y espacios", "ATENCION", MB_OK | MB_ICONERROR);
				break;
			}
			Comercio* aux1 = listaComercio;
			if (listaComercio == aux1)
			{
				listaComercio = nuevo;
			}nuevo->sgteComercio = aux1;
			EndDialog(hwnd, wPARAM);
			GuardarComercio();
			DialogBox(NULL, MAKEINTRESOURCE(DLG_Comercio), hwnd, fComercios);
		}break;
		case BTN_Alta_Comercio_Salir: {
			
			EndDialog(hwnd, wPARAM);DialogBox(NULL, MAKEINTRESOURCE(DLG_Comercio), hwnd, fComercios);
		}break;
		}
	}

				   break;
	}
	return 0;
}
LRESULT CALLBACK fLogin(HWND hwnd, UINT msg, WPARAM wPARAM, LPARAM lPARAM) {

	switch (msg)
	{
	case WM_INITDIALOG: {
		if (inicio)
		{
	    
        CargarUser();
		CargarCliente();
		CargarPromo();
	
		memset(GranTotalC, '\0', 50);
		sprintf(GranTotalC, "%.21f", GranTotal);
		
		}
		
	}
					  break;
	case WM_COMMAND: {
		switch (LOWORD(wPARAM))
		{
		case BTN_Login:
			 {
			bool band = false;
			user* actual = new user();
			actual = listaUser;
			
			while (actual != nullptr)
			{
				HWND hCombotxt = GetDlgItem(hwnd, EC_Login_User);
				HWND hCombotxt2 = GetDlgItem(hwnd, EC_Login_Pass);
					GetWindowText(hCombotxt, ActUser, 15);
					GetWindowText(hCombotxt2, ActUserPass, 15);

				if (strcmp(actual->UserName, ActUser)==0 and strcmp(actual->password, ActUserPass)==0) {
					band = true;
					break;
				}
				actual = actual->sgteUser;
			}
			if(band == true)
			{

			EndDialog(hwnd, wPARAM);
			if (actual->admin==true)
			{
             DialogBox(NULL, MAKEINTRESOURCE(DLG_Principal_Admin), hwnd, fPrincipal_Admin); 
			}
			else
			{
				DialogBox(NULL, MAKEINTRESOURCE(DLG_Principal_Cajero), hwnd, fPrincipal_Cajero);
				
			}
			
			}
			else
			{
				MessageBox(hwnd, "USUARIO O CONTRASENA INCORRECTA, POR FAVOR VERIFIQUE SUS DATOS", "ATENCION", MB_OK|MB_ICONERROR);
			}
				
			}
			break;
		case BTN_Login_Registrar:
			if (deshabilitarAux)
			{
				deshabilitar = true;
			}
			DialogBox(NULL, MAKEINTRESOURCE(DLG_Alta_User), hwnd, fLogin_Alta_Admin);

			break;
		case BTN_Login_Salir:
			if (MessageBox(hwnd, "estas seguro que deseas salir?", "ATENCION", MB_OKCANCEL) == IDOK)
			{
				SendMessage(hwnd, WM_DESTROY, 0, 0);
			}
			break;
		}
	}
		break;
	case WM_DESTROY:
		strcpy(Comer, ComerAux);
		GuardarUser();
		GuardarCliente();
		GuardarPromo();
		
		PostQuitMessage(0);
		break;
	}
	return 0;
}
LRESULT CALLBACK fLogin_Alta_Admin(HWND hwnd, UINT msg, WPARAM wPARAM, LPARAM lPARAM) {

	switch (msg)
	{
	case WM_INITDIALOG: {
		if (deshabilitar)
		  {
			
			EnableWindow(GetDlgItem(hwnd, RB_Alta_User_Admin), FALSE);
		   }
		   else {
			EnableWindow(GetDlgItem(hwnd, RB_Alta_User_Admin), TRUE);
	     	}
	
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wPARAM))
		{
		case BTN_Alta_User_Aceptar:{
			
			user* nuevo = new user();
			id++;
			
			int idaux;
			idaux = id + 1;
			
				nuevo->ID = id;
				GetDlgItemText(hwnd, EC_Alta_User_Username2, nuevo->FullnameAdmin, 50);
				strcpy(aux, nuevo->FullnameAdmin);
				if (strlen(nuevo->FullnameAdmin) < 5 || strlen(nuevo->FullnameAdmin) > 50)
				{
					MessageBox(hwnd, "El nombre solo puede contener de 5 a 50 letras", "ATENCION", MB_OK | MB_ICONERROR);
					id--;

					break;
				}
			
			GetDlgItemText(hwnd, EC_Alta_User_Username,nuevo->UserName,15);
			strcpy(aux, nuevo->UserName);
			if (!validarDuplicadoUser(hwnd))
			{
				id--;
				break;
			}
			GetDlgItemText(hwnd, EC_Alta_User_Pass, nuevo->password, 10);
			strcpy(nuevo->foto,nofoto);
	
			strcpy(nuevo->aliasAdmin, ".");
			if (IsDlgButtonChecked(hwnd, RB_Alta_User_Admin))
			{
				nuevo->admin = true;
				deshabilitar = true;
				deshabilitarAux = true;
			}
			else
			{
				nuevo->admin = false;
			}
			if (!IsDlgButtonChecked(hwnd, RB_Alta_User_Admin) and !IsDlgButtonChecked(hwnd, RB_Alta_User_Cajero))
			{
				MessageBox(hwnd, "Seleccione el tipo de usuario", "ATENCION", MB_OK | MB_ICONERROR);
				id--;
		
				break;
			}
			
			int CBlength = GetWindowTextLength(GetDlgItem(hwnd, EC_Alta_User_Username));
			if (CBlength < 5 or CBlength>15)
			{
				MessageBox(hwnd, "El nombre de usuario solo puede contener de 5 a 15 letras", "ATENCION", MB_OK | MB_ICONERROR);
				id--;
		
				break;
			}
			if (!sololetras(nuevo->UserName))
			{
				MessageBox(hwnd, "El nombre de usuario solo puede contener letras", "ATENCION", MB_OK | MB_ICONERROR);
				id--;
				
				break;
			}
			CBlength = GetWindowTextLength(GetDlgItem(hwnd, EC_Alta_User_Pass));
			if (CBlength < 3 or CBlength>10)
			{
				MessageBox(hwnd, "La contraseña solo puede contener de 3 a 10 caracteres", "ATENCION", MB_OK | MB_ICONERROR);
				id--;
				
				break;
			}
			if (!validarPass(nuevo->password))
			{
				MessageBox(hwnd, "la contraseña debe contener una letra ,un numero y un caracter especial", "ATENCION", MB_OK | MB_ICONERROR);
				id--;
		
				break;
			}
			
			user* aux1 = listaUser;
			user* aux2 = nullptr;
			while (aux1 != nullptr and aux1->ID <= idaux)
			{
				aux2 = aux1;
				aux1 = aux1->sgteUser;
			}

			if (listaUser == aux1)
			{
				listaUser = nuevo;
			}
			else
			{
				aux2->sgteUser = nuevo;
			}
			nuevo->sgteUser = aux1;
			contUser++;
			deshabilitar = false;
			EndDialog(hwnd, wPARAM);
		}
			break;
		case BTN_Alta_User_cancelar:
			EndDialog(hwnd, wPARAM);
			break;
		}
	}
	
		break;
	}
	return 0;
}
LRESULT CALLBACK mostrarListado(HWND hwnd) {
	user* actual = new user;
	int lb = SendMessage(GetDlgItem(hwnd, LB_Listado_User), LB_GETCURSEL, 0, 0);
	actual = listaUser;
	SendMessage(GetDlgItem(hwnd, LB_Listado_User), LB_GETTEXT, lb, (LPARAM)aux);
		while (actual != nullptr)
		{
			if (strcmp(aux,actual->UserName)==0)
			{
				
				
				SetDlgItemText(hwnd, EC_Listado_User_Username, actual->UserName);
				SetDlgItemText(hwnd, EC_Listado_User_Fullname, actual->FullnameAdmin);
				SetDlgItemText(hwnd, EC_Listado_User_Alias, actual->aliasAdmin);
				
				HWND hPC = GetDlgItem(hwnd, Pic_Listado_User_Foto);
				HBITMAP hImage = (HBITMAP)LoadImage(NULL, actual->foto, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				SendMessage(hPC, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage);
				break;
			}
			actual = actual->sgteUser;
		}
	return true;
}
LRESULT CALLBACK fListado_Users(HWND hwnd, UINT msg, WPARAM wPARAM, LPARAM lPARAM) {
	
	switch (msg)
	{
	case WM_INITDIALOG: {
		user* actual = new user;

		actual = listaUser;
		while (actual != nullptr)
		{
			SendMessage(GetDlgItem(hwnd, LB_Listado_User), LB_ADDSTRING, 0, (LPARAM)actual->UserName);
			actual = actual->sgteUser;
		}
		if (cajero)
		{
			EnableWindow(GetDlgItem(hwnd, BTN_Listado_User_Eliminar), FALSE);
		}
		else
		{
			EnableWindow(GetDlgItem(hwnd, BTN_Listado_User_Eliminar), TRUE);
		}
			
	}
					  break;
	case WM_COMMAND: {
		switch (LOWORD(wPARAM))
		{
		case BTN_Listado_User_Actualizar: {
			int lb = SendMessage(GetDlgItem(hwnd, LB_Listado_User), LB_GETCURSEL, 0, 0);
			SendMessage(GetDlgItem(hwnd, LB_Listado_User), LB_GETTEXT, lb, (LPARAM)aux);
			DialogBox(NULL, MAKEINTRESOURCE(DLG_Actualizar_User), hwnd, fListado_USers_Actualizar);
			}
			break;
		case BTN_Listado_User_mostrar: {
			mostrarListado(hwnd);

		}
          break;
		case BTN_Listado_User_Regresar:
			EndDialog(hwnd, wPARAM);
			break;
		case BTN_Listado_User_Eliminar: {
			strcpy(aux, "");
			int lb = SendMessage(GetDlgItem(hwnd, LB_Listado_User), LB_GETCURSEL, 0, 0);
			SendMessage(GetDlgItem(hwnd, LB_Listado_User), LB_GETTEXT, lb, (LPARAM)aux);
			if (strcmp(aux,"")==0)
			{
				MessageBox(hwnd, "no selecciono ningun usuario", "ATENCION", MB_OK|MB_ICONERROR);
				break;
			}
			if (strcmp(aux,ActUser)==0)
			{
				MessageBox(hwnd, "no puede eliminar el usuario activo,cierre sesion e intentelo con otro administrador", "ATENCION", MB_OK| MB_ICONERROR);
				break;
			}
				if (MessageBox(hwnd, "estas seguro que deseas eliminar el registro seleccionado??", "ATENCION", MB_OKCANCEL) == IDOK)
				{
					if (&listaUser != nullptr)
					{
						user* aux_del;
						user* ant = nullptr;
						aux_del = listaUser;
						strcpy(charAUXFN, aux_del->UserName);
						cambiarSpace(charAUXFN);
						while (aux_del != nullptr and strcmp(aux_del->UserName,aux)!=0)//recorrer lista
						{
							ant = aux_del;
							aux_del = aux_del->sgteUser;
							strcpy(charAUXFN, aux_del->UserName);
							cambiarSpace(charAUXFN);
						}
						if (ant == nullptr)//si el elemnto a borrar es el primero
						{
							listaUser = listaUser->sgteUser;
							contUser--;
							delete aux_del;
						}
						else//si el elemento a borrar no es el primero
						{
							ant->sgteUser = aux_del->sgteUser;
							contUser--;
							delete aux_del;
						}
					}
				}
				SendMessage(GetDlgItem(hwnd, LB_Listado_User), LB_RESETCONTENT, 0, 0);
				user* actual = new user;

				actual = listaUser;
				while (actual != nullptr)
				{
					SendMessage(GetDlgItem(hwnd, LB_Listado_User), LB_ADDSTRING, 0, (LPARAM)actual->UserName);
					actual = actual->sgteUser;
				}
			}break;
		}
	}

				   break;
	}

	
	return 0;
}
LRESULT CALLBACK fListado_USers_Actualizar(HWND hwnd, UINT msg, WPARAM wPARAM, LPARAM lPARAM) {

	switch (msg)
	{
	case WM_INITDIALOG:
	{
		SetDlgItemText(hwnd, EC_Actualizar_User_User, aux);
		bool band = false;
		user* actual = new user();
		actual = listaUser;

		while (actual != nullptr)
		{

			if (strcmp(actual->UserName, aux) == 0) {
				
				
				SetDlgItemText(hwnd, EC_Actualizar_User_FullName, actual->FullnameAdmin);
				SetDlgItemText(hwnd, EC_Actualizar_User_Alias, actual->aliasAdmin);
				HWND hPC = GetDlgItem(hwnd, Pic_Actualizar_User_foto);
				HBITMAP hImage = (HBITMAP)LoadImage(NULL, actual->foto, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				SendMessage(hPC, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage);
				break;
			}
			actual = actual->sgteUser;
		}
		if (cajero)
		{
			EnableWindow(GetDlgItem(hwnd, BTN_Actualizar_User_Modificar_Pass), FALSE);
		}
		else
		{
			EnableWindow(GetDlgItem(hwnd, BTN_Actualizar_User_Modificar_Pass), TRUE);
		}
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wPARAM))
		{
		case BTN_Actualizar_User_Modificar_foto:{
			OPENFILENAME ofn;
			
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			char DirFile[MAX_PATH] = "";
			ofn.hwndOwner = hwnd;
			ofn.lpstrFile = DirFile;
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrDefExt = "txt";
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
			ofn.lpstrFilter = "imagenes BMP\0*.BMP\0Todos los Archivos\0*.*\0";
			if (GetOpenFileName(&ofn))
			{
				strcpy(fotoAux, DirFile);
				HWND hPC = GetDlgItem(hwnd, Pic_Actualizar_User_foto);
				HBITMAP hImage = (HBITMAP)LoadImage(NULL, DirFile, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				SendMessage(hPC, STM_SETIMAGE, IMAGE_BITMAP,(LPARAM)hImage);
			}
		 }
			break;
		case BTN_Actualizar_User_Modificar_Pass:
			DialogBox(NULL, MAKEINTRESOURCE(DLG_actualizar_pass), hwnd, factualizarPass);
			break;
		case BTN_Actualizar_User_Guardar:
		{	
			bool band = false;
			user* actual = new user();
			actual = listaUser;
			char auxFN[50],auxAlias[50];
			    while (actual != nullptr){

				if (strcmp(actual->UserName, aux) == 0) {
					band = true;
					break;
				}
				actual = actual->sgteUser;
				
				}if (band == true)
				{//NO ACEPTAR ESPACIOS VACIOS
					int CBlength = GetWindowTextLength(GetDlgItem(hwnd, EC_Actualizar_User_FullName));
					
					if (CBlength < 5 or CBlength>50)
					{
						MessageBox(hwnd, "El nombre completo solo puede contener de 5 a 50 caracteres entre letras y espacios", "ATENCION", MB_OK | MB_ICONERROR);
						break;
					}
					GetWindowText(GetDlgItem(hwnd, EC_Actualizar_User_FullName), auxFN, 50);
					if(!validarFullName(auxFN)){
						MessageBox(hwnd, "El nombre completo solo puede contener letras y espacios", "ATENCION", MB_OK | MB_ICONERROR);
						break;
					}
					CBlength = GetWindowTextLength(GetDlgItem(hwnd, EC_Actualizar_User_Alias));

					if (CBlength < 3 or CBlength>10)
					{
						MessageBox(hwnd, "El alias solo puede contener de 3 a 10 caracteres entre letras y numeros", "ATENCION", MB_OK | MB_ICONERROR);
						break;
					}
					GetWindowText(GetDlgItem(hwnd, EC_Actualizar_User_Alias), auxAlias, 50);
					if (!validarAlias(auxAlias)){
						MessageBox(hwnd, "El alias solo puede contener letras y numeros", "ATENCION", MB_OK | MB_ICONERROR);
						break;
					}
					strcpy(actual->FullnameAdmin, auxFN);
					strcpy(actual->aliasAdmin, auxAlias);
					strcpy(actual->foto, fotoAux);
			}
			
			EndDialog(hwnd, wPARAM);
		}
			break;
		case BTN_Actualizar_User_Volver:
			EndDialog(hwnd, wPARAM);
			break;
			
		}
	}

				   break;
	}
	
	return 0;
	
}
LRESULT CALLBACK factualizarPass(HWND hwnd, UINT msg, WPARAM wPARAM, LPARAM lPARAM) {

	switch (msg)
	{
	case WM_COMMAND: {
		switch (LOWORD(wPARAM))
		{
		case BTN_actualizar_Pass_Aceptar:
		{
			bool band = false;
			user* actual = new user();
			actual = listaUser;
			while (actual != nullptr)
			{
				if (strcmp(actual->UserName,aux) == 0) {
					band = true;
					break;
				}
				actual = actual->sgteUser;
			}
			
			if (band == true)
			{
				GetWindowText(GetDlgItem(hwnd, EC_actualizar_Pass), ActUserPass, 15);
				int CBlength = GetWindowTextLength(GetDlgItem(hwnd, EC_actualizar_Pass));
				if (CBlength < 3 or CBlength>10)
				{
					MessageBox(hwnd, "La contraseña solo puede contener de 3 a 10 caracteres", "ATENCION", MB_OK | MB_ICONERROR);
					break;
				}
				if (!validarPass(ActUserPass))
				{
					MessageBox(hwnd, "la contraseña debe contener una letra ,un numero y un caracter especial", "ATENCION", MB_OK | MB_ICONERROR);
					break;
				}
			}
              strcpy(actual->password, ActUserPass);
               EndDialog(hwnd, wPARAM);

		}
			break;
		case TN_actualizar_Pass_Cancelar:
			EndDialog(hwnd,wPARAM);
			break;
		}
	}  break;
	}
	return 0;
}
LRESULT CALLBACK fAlta_Clientes(HWND hwnd, UINT msg, WPARAM wPARAM, LPARAM lPARAM) {

	switch (msg)
	{
	case WM_INITDIALOG: {
		
		fecha();
		SetDlgItemText(hwnd, EC_Alta_Cliente_Fecha, F);
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wPARAM))
		{
		case BTN_Alta_Cliente_Guardar: {
			char auxtel[50];
			userCliente* nuevo = new userCliente();
			idCliente++;

			int idaux;
			idaux = idCliente + 1;
				nuevo->IDCliente = idCliente;
				

			GetDlgItemText(hwnd, EC_Alta_Cliente_name, nuevo->nombreCliente, 50);
			GetDlgItemText(hwnd, EC_Alta_Cliente_Alias, nuevo->aliasCliente, 10);

			GetDlgItemText(hwnd,EC_Alta_Cliente_Telefono,nuevo->telefono,11);
			GetDlgItemText(hwnd, EC_Alta_Cliente_Email, nuevo->email, 50);
			GetDlgItemText(hwnd, EC_Alta_Cliente_Email3, nuevo->compra, 50);
			GetDlgItemText(hwnd, EC_Alta_Cliente_Fecha, nuevo->Fecha, 50);
			strcpy(aux, nuevo->nombreCliente);
			int CBlength = GetWindowTextLength(GetDlgItem(hwnd, EC_Alta_Cliente_name));
				if (CBlength < 5 or CBlength>50)
				{
					MessageBox(hwnd, "El nombre solo puede contener de 5 a 50 caracteres entre letras y espacios", "ATENCION", MB_OK | MB_ICONERROR);
					idCliente--;
					
					break;
				}
				
				if (!validarFullName(nuevo->nombreCliente)) {
					MessageBox(hwnd, "El nombre completo solo puede contener letras y espacios", "ATENCION", MB_OK | MB_ICONERROR);
					idCliente--;
		
					break;
				}
				strcpy(aux, nuevo->nombreCliente);
				if (!validarDuplicadoCliente(hwnd))
				{
					idCliente--;
					break;
				}
				CBlength = GetWindowTextLength(GetDlgItem(hwnd, EC_Alta_Cliente_Alias));

				if (CBlength < 3 or CBlength>10)
				{
					MessageBox(hwnd, "El alias solo puede contener de 3 a 10 caracteres entre letras y numeros", "ATENCION", MB_OK | MB_ICONERROR);
					idCliente--;
				
					break;
				}
	
				if (!validarAlias(nuevo->aliasCliente)) {
					MessageBox(hwnd, "El alias solo puede contener letras", "ATENCION", MB_OK | MB_ICONERROR);
					idCliente--;
			
					break;
				}
				
				if (!soloNumeros(nuevo->telefono))
				{
					MessageBox(hwnd, "El telefono solo puede contener numeros", "ATENCION", MB_OK | MB_ICONERROR);
					idCliente--;
				
					break;
				}
				CBlength = GetWindowTextLength(GetDlgItem(hwnd, EC_Alta_Cliente_Telefono));

				if (CBlength!=10)
				{
					MessageBox(hwnd, "El telefono debe contener exactamente 10 numeros", "ATENCION", MB_OK | MB_ICONERROR);
					idCliente--;
			
					break;
				}
				if(!validarEmail(nuevo->email)){
					MessageBox(hwnd, "Formato de Email incorrecto,ejemplo: usuario@email.com", "ATENCION", MB_OK | MB_ICONERROR);
					idCliente--;
			;
					break;
				}
				CBlength = GetWindowTextLength(GetDlgItem(hwnd, EC_Alta_Cliente_Email));
				if (CBlength < 5 or CBlength>25)
				{
					MessageBox(hwnd, "El email solo puede contener de 5 a 25 caracteres ", "ATENCION", MB_OK | MB_ICONERROR);
					idCliente--;
					break;
				}
				


			userCliente* aux1 = listaClientes;
			userCliente* aux2 = nullptr;
			while (aux1 != nullptr and aux1->IDCliente <= idaux)
			{
				aux2 = aux1;
				aux1 = aux1->sgteUserCliente;
			}

			if (listaClientes == aux1)
			{
				listaClientes = nuevo;
			}
			else
			{
				aux2->sgteUserCliente = nuevo;
			}
			nuevo->sgteUserCliente = aux1;
			contCliente++;
			EndDialog(hwnd, wPARAM);
		}
			break;
		case BTN_Alta_Cliente_Regresar:
			EndDialog(hwnd, wPARAM);
			break;
		}
	}break;		 
	}  
	return 0;
}
LRESULT CALLBACK fActualizar_Clientes(HWND hwnd, UINT msg, WPARAM wPARAM, LPARAM lPARAM) {

	switch (msg)
	{
	case WM_INITDIALOG: {
		SetDlgItemText(hwnd, EC_Actualizar_Clientes_Nombre, aux);
		bool band = false,valid;
		userCliente* actual = new userCliente();
		actual = listaClientes;

		while (actual != nullptr)
		{
			strcpy(charAUXFN, actual->nombreCliente);
			cambiarSpace(charAUXFN);
			if (strcmp(charAUXFN, aux) == 0) {
				SetDlgItemText(hwnd, EC_Actualizar_Clientes_Fecha, actual->Fecha);
				SetDlgItemText(hwnd, EC_Actualizar_Clientes_Alias, actual->aliasCliente);
				SetDlgItemText(hwnd, EC_Actualizar_Clientes_Telefono, actual->telefono);
				SetDlgItemText(hwnd, EC_Actualizar_Clientes_Email, actual->email);
				break;
			}
			actual = actual->sgteUserCliente;
		}
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wPARAM))
		{
		case BTN_Actualizar_Clientes_Guardar:
			{
			bool band = false;
			char auxiliar[50];
			userCliente* actual = new userCliente();
			actual = listaClientes;
			while (actual != nullptr){
				strcpy(charAUXFN, actual->nombreCliente);
				cambiarSpace(charAUXFN);
				if (strcmp(charAUXFN, aux) == 0) {
					band = true;
					break;
				}
				actual = actual->sgteUserCliente;
				
				}if (band == true)
				{//NO ACEPTAR ESPACIOS VACIOS
					int CBlength = GetWindowTextLength(GetDlgItem(hwnd, EC_Actualizar_Clientes_Alias));

					if (CBlength < 3 or CBlength>10)
					{
						MessageBox(hwnd, "El alias solo puede contener de 3 a 10 caracteres entre letras y espacios", "ATENCION", MB_OK | MB_ICONERROR);
						break;
					}
					GetDlgItemText(hwnd, EC_Actualizar_Clientes_Alias, auxiliar, 10);
					if (!validarAlias(auxiliar)) {
						MessageBox(hwnd, "El alias solo puede contener letras", "ATENCION", MB_OK | MB_ICONERROR);
						
						break;
					}
					GetDlgItemText(hwnd, EC_Actualizar_Clientes_Telefono, auxiliar, 11);
					if (!soloNumeros(auxiliar))
					{
						MessageBox(hwnd, "El telefono solo puede contener numeros", "ATENCION", MB_OK | MB_ICONERROR);
					
						break;
					}
					CBlength = GetWindowTextLength(GetDlgItem(hwnd, EC_Actualizar_Clientes_Telefono));

					if (CBlength != 10)
					{
						MessageBox(hwnd, "El telefono debe contener exactamente 10 numeros", "ATENCION", MB_OK | MB_ICONERROR);
					
						break;
					}
					GetDlgItemText(hwnd, EC_Actualizar_Clientes_Email, auxiliar, 50);
					if (!validarEmail(auxiliar)) {
						MessageBox(hwnd, "Formato de Email incorrecto,ejemplo: usuario@email.com", "ATENCION", MB_OK | MB_ICONERROR);
					
						break;
					}
					CBlength = GetWindowTextLength(GetDlgItem(hwnd, EC_Actualizar_Clientes_Email));
					if (CBlength < 5 or CBlength>25)
					{
						MessageBox(hwnd, "El email solo puede contener de 5 a 25 caracteres ", "ATENCION", MB_OK | MB_ICONERROR);
					
						break;
					}
					GetDlgItemText(hwnd, EC_Actualizar_Clientes_Alias, actual->aliasCliente, 10);
					 GetDlgItemText(hwnd, EC_Actualizar_Clientes_Telefono, actual->telefono,11);
					GetDlgItemText(hwnd, EC_Actualizar_Clientes_Email, actual->email, 50);
			    }
			EndDialog(hwnd, wPARAM);
		}
		break;
		case BTN_Actualizar_Clientes_Cancelar:
			EndDialog(hwnd, wPARAM);
			break;
		
		}
	}

				   break;
	}
	return 0;

}
LRESULT CALLBACK mostrarListadoClientes(HWND hwnd) {
	userCliente* actual = new userCliente;

	actual = listaClientes;
	int lb = SendMessage(GetDlgItem(hwnd, LB__Listado_Clientes), LB_GETCURSEL, 0, 0);
	actual = listaClientes;
	SendMessage(GetDlgItem(hwnd, LB__Listado_Clientes), LB_GETTEXT, lb, (LPARAM)aux);
	cambiaraGuion(aux);
	while (actual != nullptr)
	{
		if (strcmp(aux, actual->nombreCliente) == 0)
		{
			cambiarSpace(aux);
			SetDlgItemText(hwnd, EC_Listado_Clientes_Nombre, aux);
			SetDlgItemText(hwnd, EC_Listado_Clientes_Alias, actual->aliasCliente);
			SetDlgItemText(hwnd, EC_Listado_Clientes_Email, actual->email);
			SetDlgItemText(hwnd, EC_Listado_Clientes_Fecha, actual->Fecha);
			SetDlgItemText(hwnd, EC_Listado_Clientes_Telefono, actual->telefono);
			break;
		}
		actual = actual->sgteUserCliente;
	}
	return true;
}
LRESULT CALLBACK fListado_Clientes(HWND hwnd, UINT msg, WPARAM wPARAM, LPARAM lPARAM) {

	switch (msg)
	{
	case WM_INITDIALOG: {
		userCliente* actual = new userCliente;

		actual = listaClientes;
		while (actual != nullptr)
		{
			strcpy(charAUXFN,actual->nombreCliente);
			cambiarSpace(charAUXFN);
			SendMessage(GetDlgItem(hwnd, LB__Listado_Clientes), LB_ADDSTRING, 0, (LPARAM)charAUXFN);
			actual = actual->sgteUserCliente;
		}
		if (cajero)
		{
			EnableWindow(GetDlgItem(hwnd, BTN_Listado_Clientes_Eliminar), FALSE);
		}
		else
		{
			EnableWindow(GetDlgItem(hwnd, BTN_Listado_Clientes_Eliminar), TRUE);
		}
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wPARAM))
		{
		case BTN_Listado_Clientes_Modificar:{
			int lb = SendMessage(GetDlgItem(hwnd, LB__Listado_Clientes), LB_GETCURSEL, 0, 0);
			SendMessage(GetDlgItem(hwnd, LB__Listado_Clientes), LB_GETTEXT, lb, (LPARAM)aux);
			DialogBox(NULL, MAKEINTRESOURCE(DLG_Actualizar_Cliente), hwnd, fActualizar_Clientes);
		}
			
			break;
		case BTN_Listado_Clientes_Eliminar:
		{
			strcpy(aux, "");
			
                 int lb = SendMessage(GetDlgItem(hwnd, LB__Listado_Clientes), LB_GETCURSEL, 0, 0);
				SendMessage(GetDlgItem(hwnd, LB__Listado_Clientes), LB_GETTEXT, lb, (LPARAM)aux);
				if (strcmp(aux, "") == 0)	{
				MessageBox(hwnd, "no selecciono ningun usuario", "ATENCION", MB_OK);
				break;
			     }
			if (MessageBox(hwnd, "estas seguro que deseas eliminar el registro seleccionado??", "ATENCION", MB_OKCANCEL) == IDOK)
			{
				
				if (&listaClientes != nullptr)
				{
					userCliente* aux_del;
					userCliente* ant = nullptr;
					aux_del = listaClientes;
					strcpy(charAUXFN, aux_del->nombreCliente);
					cambiarSpace(charAUXFN);
					while (aux_del != nullptr and strcmp(charAUXFN, aux) != 0)//recorrer lista
					{
						ant = aux_del;
						aux_del = aux_del->sgteUserCliente;
						strcpy(charAUXFN, aux_del->nombreCliente);
						cambiarSpace(charAUXFN);
					}
					if (ant == nullptr)//si el elemnto a borrar es el primero
					{
						listaClientes = listaClientes->sgteUserCliente;
						
						contCliente--;
						delete aux_del;
					}
					else//si el elemento a borrar no es el primero
					{
						ant->sgteUserCliente = aux_del->sgteUserCliente;
						contCliente--;
					
						delete aux_del;
					}
				}
			}

		}
			break;
		case BTN_Listado_Cliente_Mostrar: {
			mostrarListadoClientes(hwnd);
		}break;
		case BTN_Listado_Clientes_Regresar:
			EndDialog(hwnd, wPARAM);
			break;
		}
	}

				   break;
	}
	return 0;

}
LRESULT CALLBACK fAlta_Promo(HWND hwnd, UINT msg, WPARAM wPARAM, LPARAM lPARAM) {
	
	switch (msg)
	{
	case WM_INITDIALOG:{
		
		fecha();
		SetDlgItemText(hwnd, EC_Alta_Promo_Fecha, F);
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wPARAM))
		{
		case BTN_Alta_Promos_Guardar:
		{
			Promo* nuevo = new Promo();
			idPromo++;
			char flotante[20],auxiliar[50];
			float auxiliarF=0;
			int idaux;
			idaux = idPromo + 1;

		
				nuevo->IDPromo = idPromo;
				
			GetDlgItemText(hwnd, EC_Alta_Promo_Name, nuevo->nombrePromo, 50);
			
			int CBlength = GetWindowTextLength(GetDlgItem(hwnd, EC_Alta_Promo_Name));
			if (CBlength < 5 or CBlength>50)
			{
				MessageBox(hwnd, "El nombre solo puede contener de 5 a 50 caracteres entre letras y espacios", "ATENCION", MB_OK | MB_ICONERROR);
				idPromo--;
				break;
			}

			if (!validarFullName(nuevo->nombrePromo)) {
				MessageBox(hwnd, "El nombre completo solo puede contener letras y espacios", "ATENCION", MB_OK | MB_ICONERROR);
				idPromo--;
				break;
			}
			strcpy(aux, nuevo->nombrePromo);
			if (!validarDuplicadoPromo(hwnd))
			{
				idPromo--;
				break;
			}
			GetDlgItemText(hwnd, EC_Alta_Promo_Fecha, nuevo->fechaPromo, 50);
			memset(flotante, '\0', 20);
			GetDlgItemText(hwnd, EC_Alta_Promo_Monto, flotante, 20);
			auxiliarF = strtod(flotante, NULL);
			if (auxiliarF<100 or !soloFloat(flotante))
			{
				MessageBox(hwnd, "el monto minimo de una promocion es de $100", "ATENCION", MB_OK | MB_ICONERROR);
				idPromo--;
				break;
			}
				auxiliarF = GetDlgItemInt(hwnd, EC_Alta_Promo_Descuento, NULL, FALSE);
				if (auxiliarF<1 or auxiliarF>100)
				{
					MessageBox(hwnd, "el descuento deben ser numeros enteros desde el 1 al 100", "ATENCION", MB_OK | MB_ICONERROR);
					idPromo--;
					break;
				}
				if (IsDlgButtonChecked(hwnd, RB_Alta_Promo_Status_Activo))
				{
					nuevo->status= true;
					activas++;

				}
				if (IsDlgButtonChecked(hwnd, RB_Alta_Promo_Status_Inactivo))
				{
					nuevo->status = false;
					inactivas++;
				}
				if (!IsDlgButtonChecked(hwnd, RB_Alta_Promo_Status_Activo) and !IsDlgButtonChecked(hwnd, RB_Alta_Promo_Status_Inactivo))
				{
					MessageBox(hwnd, "Seleccione el status de la promocion", "ATENCION", MB_OK | MB_ICONERROR);
					idPromo--;
					break;
				}
			nuevo->montoPromo = strtod(flotante,NULL);
			nuevo->descuentoPromo = GetDlgItemInt(hwnd, EC_Alta_Promo_Descuento, NULL, FALSE);

			Promo* aux1 = listaPromo;
			Promo* aux2 = nullptr;
			while (aux1 != nullptr and aux1->IDPromo <= idaux)
			{
				aux2 = aux1;
				aux1 = aux1->sgtePromo;
			}

			if (listaPromo == aux1)
			{
				listaPromo = nuevo;
			}
			else
			{
				aux2->sgtePromo = nuevo;
			}
			nuevo->sgtePromo = aux1;

			EndDialog(hwnd, wPARAM);
		}
			break;
		case BTN_Alta_Promos_Regresar:
			EndDialog(hwnd, wPARAM);
			break;
		}
	}

				   break;
	}
	return 0;
}
LRESULT CALLBACK mostrarListadoPromo(HWND hwnd) {
	Promo* actual = new Promo;
	actual = listaPromo;
	char flotante[50];
	int lb = SendMessage(GetDlgItem(hwnd, LB_Listado_promos), LB_GETCURSEL, 0, 0);
	actual = listaPromo;
	SendMessage(GetDlgItem(hwnd, LB_Listado_promos), LB_GETTEXT, lb, (LPARAM)aux);
	cambiaraGuion(aux);
	while (actual != nullptr)
	{
		if (strcmp(aux, actual->nombrePromo) == 0)
		{
			cambiarSpace(aux);
			SetDlgItemText(hwnd, EC_Listado_Promo_Name, aux);
			SetDlgItemText(hwnd, EC_Listado_Promo_Fecha, actual->fechaPromo);
			if (actual->status==true)
			{
              SetDlgItemText(hwnd, EC_Listado_Promo_Status, "activo");
			}
			else
			{
				SetDlgItemText(hwnd, EC_Listado_Promo_Status, "inactivo");
			}
			memset(flotante, '\0', 50);
			sprintf(flotante, "%.21f", actual->montoPromo);
			SetDlgItemText(hwnd, EC_Listado_Promo_Monto, flotante);
			SetDlgItemInt(hwnd, EC_Listado_Promo_Descuento, actual->descuentoPromo, FALSE);
			break;
		}
		actual = actual->sgtePromo;
	}
	return true;
}
LRESULT CALLBACK fActualizar_Promo(HWND hwnd, UINT msg, WPARAM wPARAM, LPARAM lPARAM) {

	switch (msg)
	{
	case WM_INITDIALOG: {
		Promo* actual = new Promo;
		actual = listaPromo;

		while (actual != nullptr)
		{
			strcpy(charAUXFN, actual->nombrePromo);
				cambiarSpace(charAUXFN);
			if (strcmp(aux, charAUXFN) == 0)
			{
				
				SetDlgItemText(hwnd, EC_Actualizar_Promo_Name,charAUXFN);
				SetDlgItemText(hwnd, EC_Actualizar_Promo_Fecha, actual->fechaPromo);

				SetDlgItemInt(hwnd, EC_Actualizar_Promo_Descuento, actual->descuentoPromo, FALSE);
				SetDlgItemInt(hwnd, EC_Actualizar_Promo_Monto, actual->montoPromo, FALSE);
				break;
			}
			actual = actual->sgtePromo;
		}
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wPARAM))
		{
		case BTN_Actualizar_Promos_Guardar:	{
			
			bool band = false;
			Promo* actual = new Promo();
			actual = listaPromo;
			while (actual != nullptr) {
				strcpy(charAUXFN, actual->nombrePromo);
				cambiarSpace(charAUXFN);
				if (strcmp(charAUXFN, aux) == 0) {
					band = true;
					break;
				}
				actual = actual->sgtePromo;
			}
			if (band == true){
				//NO ACEPTAR ESPACIOS VACIOS
				if (IsDlgButtonChecked(hwnd, RB_Actualizar_Promo_Status_Activo))
				{
					actual->status = true;
					activas++;
					inactivas--;

				}
				if(IsDlgButtonChecked(hwnd,RB_Actualizar_Promo_Status_Inactivo))
				{
					actual->status = false;
					inactivas++;
					activas--;
				}
				if (!IsDlgButtonChecked(hwnd, RB_Actualizar_Promo_Status_Activo) and !IsDlgButtonChecked(hwnd, RB_Actualizar_Promo_Status_Inactivo))
				{
					MessageBox(hwnd, "Seleccione el status de la promocion", "ATENCION", MB_OK | MB_ICONERROR);
					break;
				}
			}
			EndDialog(hwnd, wPARAM);
		}
			break;
		case BTN_Actualizar_Promos_Regresar:
			EndDialog(hwnd, wPARAM);
			break;
		}
	}

				   break;
	}
	return 0;

}
LRESULT CALLBACK fListado_Promos(HWND hwnd, UINT msg, WPARAM wPARAM, LPARAM lPARAM) {

	switch (msg)
	{
	case WM_INITDIALOG: {
		Promo* actual = new Promo;

		actual = listaPromo;
		while (actual != nullptr)
		{
			strcpy(charAUXFN, actual->nombrePromo);
				cambiarSpace(charAUXFN);
			SendMessage(GetDlgItem(hwnd, LB_Listado_promos), LB_ADDSTRING, 0, (LPARAM)charAUXFN);
			actual = actual->sgtePromo;
		}
		if (cajero)
		{
			EnableWindow(GetDlgItem(hwnd, BTN_Listado_Promos_Eliminar), FALSE);
		}
		else
		{
			EnableWindow(GetDlgItem(hwnd, BTN_Listado_Promos_Eliminar), TRUE);
		}
	}break;
	case WM_COMMAND: {
		switch (LOWORD(wPARAM))
		{
		case BTN_Listado_Promos_Guardar:
		{
			mostrarListadoPromo(hwnd);
		}break;
		case BTN_Listado_Promos_Eliminar:
		{
			strcpy(aux, "");
			
			int lb = SendMessage(GetDlgItem(hwnd, LB_Listado_promos), LB_GETCURSEL, 0, 0);
				SendMessage(GetDlgItem(hwnd, LB_Listado_promos), LB_GETTEXT, lb, (LPARAM)aux);
				if (strcmp(aux, "") == 0){
				MessageBox(hwnd, "no selecciono ninguna promocion", "ATENCION", MB_OK);
				break;
			    }
			if (MessageBox(hwnd, "estas seguro que deseas eliminar el registro seleccionado??", "ATENCION", MB_OKCANCEL) == IDOK)
			{
				
				if (&listaPromo != nullptr)
				{
					Promo* aux_del;
					Promo* ant = nullptr;
					aux_del = listaPromo;
					strcpy(charAUXFN, aux_del->nombrePromo);
					cambiarSpace(charAUXFN);
					while (aux_del != nullptr and strcmp(charAUXFN, aux) != 0)//recorrer lista
					{
						ant = aux_del;
						aux_del = aux_del->sgtePromo;
						strcpy(charAUXFN, aux_del->nombrePromo);
						cambiarSpace(charAUXFN);
					}
					if (ant == nullptr)//si el elemnto a borrar es el primero
					{
						listaPromo = listaPromo->sgtePromo;
						if ((aux_del->status)==true)
						{
							activas--;
						}
						else
						{
							inactivas--;
						}
						delete aux_del;
					}
					else//si el elemento a borrar no es el primero
					{
						ant->sgtePromo = aux_del->sgtePromo;
						if ((aux_del->status) == true)
						{
							activas--;
						}
						else
						{
							inactivas--;
						}
						
						delete aux_del;
					}
				}
			}
		}
		break;
		case BTN_Listado_Promo_Actualizar: {
			int lb = SendMessage(GetDlgItem(hwnd, LB__Listado_Clientes), LB_GETCURSEL, 0, 0);
			SendMessage(GetDlgItem(hwnd, LB__Listado_Clientes), LB_GETTEXT, lb, (LPARAM)aux);
			DialogBox(NULL, MAKEINTRESOURCE(DLG_Actualizar_Promo), hwnd, fActualizar_Promo);
		}
			
			break;
		case BTN_Listado_Promos_Regresar:
			EndDialog(hwnd, wPARAM);
			break;
		}
		
	}

				   break;
	}
	return 0;

}
void AplicarPromo() {
	Promo* actual = new Promo;
	actual = listaPromo;
	float descuentoo;
	int cantdesc[20];

	int contProms=0;

	descuento = false;
	bool band = false;
	while (actual != nullptr)
	{
		if (actual->montoPromo <= totalAux and actual->status == true)
		{
			band = true;
			contProms++;
			cantdesc[contProms] = actual->descuentoPromo;
		}
		actual = actual->sgtePromo;
	}

	if (band == true)
	{
		descuento = true;
		descuentoo = (cantdesc[contProms]) * totalAux / 100;
		descuentoaplicado = cantdesc[contProms];
		nuevoTotal = totalAux - descuentoo;
	}
	else
	{
		
		nuevoTotal = totalAux;
	}
}


LRESULT CALLBACK mostrarDashboard(HWND hwnd) {
	char buffer[20];
	SetDlgItemInt(hwnd, EC_Principal_Admin_Users, contUser, FALSE);
	SetDlgItemInt(hwnd, EC_Principal_Admin_Cantidad_Clientes, contCliente, FALSE);
	SetDlgItemInt(hwnd, EC_Principal_Admin_Cantidad_Promos, (activas+inactivas), FALSE);
	SetDlgItemInt(hwnd, EC_Principal_Admin_Cantidad_Promos_Activas, activas, FALSE);
	SetDlgItemInt(hwnd, EC_Principal_Admin_Cantidad_Promos_Inactivas, inactivas, FALSE);

	SetDlgItemText(hwnd,EC_Principal_Admin_Cantidad_ConsumosTotal, GranTotalC);
	SetDlgItemText(hwnd, EC_Principal_Admin_User, ActUser);
	SetDlgItemText(hwnd, EC_Principal_Admin_User2, Comer);
	bool band = false;
	user* actual = new user();
	actual = listaUser;

	while (actual != nullptr)
	{

		if (strcmp(actual->UserName, ActUser) == 0) {
			HWND hPC = GetDlgItem(hwnd, Pic_Principal_User_Foto);
			HBITMAP hImage = (HBITMAP)LoadImage(NULL, actual->foto, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
			SendMessage(hPC, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage);
			break;
		}
		actual = actual->sgteUser;
	}
	/*memset(buffer, '\0', 20);
	sprintf(buffer,"%.21f",)*/
	return true;
}
LRESULT CALLBACK mostrarDashboardC(HWND hwnd) {
	char buffer[20];
	SetDlgItemInt(hwnd, EC_Principal_Cajero_Cantidad_Clientes, contCliente, FALSE);
	SetDlgItemInt(hwnd, EC_Principal_Cajero_Cantidad_Promos, (activas + inactivas), FALSE);
	SetDlgItemInt(hwnd, EC_Principal_Cajero_Cantidad_Promos_Activas, activas, FALSE);
	SetDlgItemInt(hwnd, EC_Principal_Cajero_Cantidad_Promos_Inactivas, inactivas, FALSE);

	SetDlgItemText(hwnd, EC_Principal_Cajero_User2, Comer);
	SetDlgItemText(hwnd, EC_Principal_Cajero_Cantidad_ConsumosTotal, GranTotalC);
	SetDlgItemText(hwnd, EC_Principal_Cajero_User, ActUser);
	bool band = false;
	user* actual = new user();
	actual = listaUser;

	while (actual != nullptr)
	{

		if (strcmp(actual->UserName, ActUser) == 0) {
			HWND hPC = GetDlgItem(hwnd, Pic_Principal_User_Foto);
			HBITMAP hImage = (HBITMAP)LoadImage(NULL, actual->foto, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
			SendMessage(hPC, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImage);
			break;
		}
		actual = actual->sgteUser;
	}
	/*memset(buffer, '\0', 20);
	sprintf(buffer,"%.21f",)*/
	return true;
}
LRESULT CALLBACK fPrincipal_Admin(HWND hwnd, UINT msg, WPARAM wPARAM, LPARAM lPARAM) {

	
	switch (msg)
	{
	case WM_INITDIALOG:{
		mostrarDashboard(hwnd);
		deshabilitar = false;
	}
	  break;
	case WM_COMMAND: {
		switch (LOWORD(wPARAM))
		{
		case CMenu_GUARDARCOMO_CSV: {
			GuardarExcel();
			MessageBox(hwnd, "GUARDADO EXITOSO", "ATENCION", MB_OK | MB_ICONASTERISK);
		}break;
		case Menu_USUARIOS_ALTA:
			DialogBox(NULL, MAKEINTRESOURCE(DLG_Alta_User), hwnd, fLogin_Alta_Admin);
			mostrarDashboard(hwnd);
			break;
		case Menu_USUARIOS_LISTADO:
			DialogBox(NULL, MAKEINTRESOURCE(DLG_Listado_User), hwnd, fListado_Users);
			mostrarDashboard(hwnd);
			break;
		case Menu_CLIENTES_ALTA:
			DialogBox(NULL, MAKEINTRESOURCE(DLG_Alta_Clientes), hwnd, fAlta_Clientes);
			mostrarDashboard(hwnd);
			break;
		case Menu_CLIENTES_LISTADO:
			DialogBox(NULL, MAKEINTRESOURCE(DLG_Listado_Clientes), hwnd, fListado_Clientes);
			mostrarDashboard(hwnd);
			break;
		case Menu_PROMOCIONES_ALTA:
			DialogBox(NULL, MAKEINTRESOURCE(DLG_Alta_Promociones), hwnd, fAlta_Promo);
			mostrarDashboard(hwnd);
			break;
		case Menu_PROMOCIONES_LISTADO:
			DialogBox(NULL, MAKEINTRESOURCE(DLG_Listado_Promos), hwnd, fListado_Promos);
			mostrarDashboard(hwnd);
			break;
	
		case BTN_Principal_Admin_unlogin:
			EndDialog(hwnd, wPARAM);
			inicio = false;
			DialogBox(NULL, MAKEINTRESOURCE(DLG_Login), hwnd, fLogin);
			break;
		}
	}
		break;
	}
	
	return 0;
	

}
LRESULT CALLBACK fPrincipal_Cajero(HWND hwnd, UINT msg, WPARAM wPARAM, LPARAM lPARAM) {


	switch (msg)
	{
	case WM_INITDIALOG: {
		mostrarDashboardC(hwnd);
		cajero = true;
		deshabilitar = false;
	}
					  break;
	case WM_COMMAND: {
		switch (LOWORD(wPARAM))
		{
		case CCMenu_GUARDARCOMO_CSV: {
			GuardarExcel();
			MessageBox(hwnd, "GUARDADO EXITOSO", "ATENCION", MB_OK | MB_ICONASTERISK);
			break;
		}break;
		case CMenu_USUARIOS_LISTADO:
			DialogBox(NULL, MAKEINTRESOURCE(DLG_Listado_User), hwnd, fListado_Users);
			mostrarDashboardC(hwnd);
			break;
		case CMenu_CLIENTES_ALTA:
			DialogBox(NULL, MAKEINTRESOURCE(DLG_Alta_Clientes), hwnd, fAlta_Clientes);
			mostrarDashboardC(hwnd);
			break;
		case CMenu_CLIENTES_LISTADO:
			DialogBox(NULL, MAKEINTRESOURCE(DLG_Listado_Clientes), hwnd, fListado_Clientes);
			mostrarDashboardC(hwnd);
			break;
		case CMenu_PROMOCIONES_ALTA:
			DialogBox(NULL, MAKEINTRESOURCE(DLG_Alta_Promociones), hwnd, fAlta_Promo);
			mostrarDashboardC(hwnd);
			break;
		case CMenu_PROMOCIONES_LISTADO:
			DialogBox(NULL, MAKEINTRESOURCE(DLG_Listado_Promos), hwnd, fListado_Promos);
			mostrarDashboardC(hwnd);
			break;
			break;
		case BTN_Principal_Cajero_unlogin:
			EndDialog(hwnd, wPARAM);
			inicio = false;
			cajero = false;
			DialogBox(NULL, MAKEINTRESOURCE(DLG_Login), hwnd, fLogin);
			break;
		}
	}
				   break;
	}

	return 0;


}
