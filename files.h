#ifndef FILES_H
#define FILES_H

#include <QString>

class Files
{
public:
    Files();

    void AddDinero(int dinero);
    //con el Data.txt, busca el dinero y le agrega, tambien crear gastarDinero(int cantidad);

    void AddItemInventario(QString Nombre);
        //con el parametro Nombre, buscara en el archivo si existe un item asi, si existe, busca la Cantidad de ese item, y le aumenta 1 a su Valor

    void UseItem(QString Nombre);
        //si la cantidad del nombre es mayor a 0 (si tenemos del item) , le resta 1 a su valor



private:

    void crearArchivos(); //.txt
    //Carpeta Archivos
        /*Data.txt ---> Dinero: int
                        Nivel: int
        */


        //Inventario.txt --> (QString, QString, QString,int,int) --> Nombre,Rareza,RutaImagen,UsableCombate,Cantidad)



    void CrearInventarioVacio();//Inventario.txt
        //cantidad siempre inicia en 0,
            //como prueba puedes crear de item una poscion, una gemaVerde, una poscion, etc...

        //creame los getters de todo

};

#endif // FILES_H
