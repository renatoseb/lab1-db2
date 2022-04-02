#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <climits>

using namespace std;

void readFromConsole(char buffer[], int size)
{
    string temp;
    cin >> temp;
    for (int i = 0; i < size; i++)
        buffer[i] = (i < temp.size()) ? temp[i] : ' ';
    buffer[size - 1] = '\0';
    cin.clear();
}

struct Alumno
{
    char codigo[5];
    char nombre[11];
    char apellidos[20];
    char carrera[15];

    int ciclo;
    float mensualidad;

    void inputData()
    {
        readFromConsole((char *)this->codigo, 5);
        readFromConsole((char *)this->nombre, 11);
        readFromConsole((char *)this->apellidos, 20);
        readFromConsole((char *)this->carrera, 15);
        cin >> ciclo;
        cin >> mensualidad;
    }

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
        cout << "Ciclo: " << ciclo << endl;
        cout << "Mensualidad: " << mensualidad << endl;
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
    cout << "Begin \n";
    stream.write((char *)&p.codigo, sizeof(p.codigo));
    stream.write((char *)&p.nombre, sizeof(p.nombre));
    stream.write((char *)&p.apellidos, sizeof(p.apellidos));
    stream.write((char *)&p.carrera, sizeof(p.carrera));

    char ciclo_char[5];
    sprintf(ciclo_char, "%d", p.ciclo);
    stream.write((char *)&ciclo_char, 5);

    char mensualidad_char[10];
    snprintf(mensualidad_char, 10, "%f", p.mensualidad);
    stream.write((char *)mensualidad_char, 10);

    stream << "\n";
    stream << flush;
    return stream;
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

        inFile.open(this->filename, ios::in | ios::binary);
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
        ofstream outFile(this->filename, ios::app | ios::binary);

        // outFile.write((char *)&record, sizeof(Alumno));
        if (outFile.is_open())
        {
            outFile << record;
            outFile.close();
        }
        else
            cerr << "No se pudo abrir el archivo.\n";

        outFile.close();
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
    FixedRecord fix("datos1.dat");
    /*
    vector<Alumno> alumnos = fix.load();
    for (auto item : alumnos)
        item.printAlumno();
    Alumno readed = fix.readRecord(2);
    readed.printAlumno();
*/
    Alumno record;
    record.inputData();
    record.printAlumno();
    fix.add(record);
}