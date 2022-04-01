#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <climits>

using namespace std;

struct Alumno
{
    char codigo[5];
    char nombre[11];
    char apellidos[20];
    char carrera[15];

    void printAlumno()
    {
        cout << "Codigo: ";
        for (auto i : codigo)
            cout << i;
        cout << endl;

        cout << "Nombre: ";
        for (auto i : nombre)
            cout << i;
        cout << endl;

        cout << "Apellidos: ";
        for (auto i : apellidos)
            cout << i;
        cout << endl;

        cout << "Carrera: ";
        for (auto i : carrera)
            cout << i;
        cout << endl;
    }
};

istream &operator>>(istream &stream, Alumno &p)
{
    // Leemos el los records y los almacenamos
    stream.read((char *)&p.codigo, 5);
    stream.read((char *)&p.nombre, 11);
    stream.read((char *)&p.apellidos, 20);
    stream.read((char *)&p.carrera, 15);

    // Dos .get() porque uno lee el salto de línea y otro lee el primer caracter.
    stream.get();
    stream.get();
    return stream;
}

ostream &operator<<(ostream &stream, Alumno &p)
{
    stream << p.codigo << " ";
    stream << p.nombre << " ";
    stream << p.apellidos << " ";
    stream << p.carrera << " ";
    stream << "\n";
    stream << flush;
    return stream;
}

void readFromConsole(char buffer[], int size)
{
    string temp;
    cin >> temp;
    for (int i = 0; i < size; i++)
        buffer[i] = (i < temp.size()) ? temp[i] : ' ';
    buffer[size - 1] = '\0';
    cin.clear();
}

class FixedRecord
{
private:
    string filename;

public:
    FixedRecord(string filename)
    {
        this->filename = filename;
    }

    vector<Alumno> load()
    {
        ifstream inFile;
        vector<Alumno> alumnos;

        inFile.open(this->filename);
        if (inFile.is_open())
        {
            Alumno temp;
            while (!inFile.eof())
            {
                inFile >> temp;
                alumnos.push_back(temp);
            }
            inFile.close();
            cout << endl;
        }
        else
            cerr << "No se pudo abrir el archivo.\n";

        return alumnos;
    }

    void add(Alumno record)
    {
        ofstream outFile;
        outFile.open(this->filename, ios::app);

        if (outFile.is_open())
        {
            outFile << record;
            outFile.close();
        }
        else
            cerr << "No se pudo abrir el archivo.\n";
    }

    Alumno readRecord(int pos)
    {
        ifstream file(this->filename, ios::app | ios::binary);
        if (!file.is_open())
            cout << "No se pudo abrir el archivo.\n";
        pos--;
        Alumno record;
        cout << "Position in bytes: " << pos * sizeof(Alumno) << endl;
        file.seekg((pos * sizeof(Alumno)) + (2 * pos), ios::beg); // fixed length record
        file.read((char *)&record.codigo, 5);
        file.read((char *)&record.nombre, 11);
        file.read((char *)&record.apellidos, 20);
        file.read((char *)&record.carrera, 15);
        file.get();
        file.close();

        return record;
    }
};

int main()
{
    FixedRecord fix("datos1.txt");
    vector<Alumno> alumnos = fix.load();
    for (auto item : alumnos)
        item.printAlumno();

    Alumno readed = fix.readRecord(12);
    readed.printAlumno();

    Alumno record;
    readFromConsole(record.codigo, 5);
    readFromConsole(record.nombre, 11);
    readFromConsole(record.apellidos, 20);
    readFromConsole(record.carrera, 15);
    record.printAlumno();
    fix.add(record);

    return 0;
}