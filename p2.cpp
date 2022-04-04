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
    char ciclo_array[5];
    stream.read((char *)&ciclo_array, 5);
    p.ciclo = atoi(ciclo_array);

    char mensualidad_array[10];
    stream.read((char *)&mensualidad_array, 10);
    p.mensualidad = atof(mensualidad_array);

    // Dos .get() porque uno lee el salto de línea y otro lee el primer caracter.
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

    // DONE, but FIXME
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

    // DONE
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

    // DONE
    Alumno readRecord(int pos)
    {
        ifstream file(this->filename, ios::binary);
        if (!file.is_open())
            cout << "No se pudo abrir el archivo.\n";
        pos--;

        Alumno record;
        int size_in_bytes = 0;

        size_in_bytes += sizeof(record.codigo);
        size_in_bytes += sizeof(record.nombre);
        size_in_bytes += sizeof(record.apellidos);
        size_in_bytes += sizeof(record.carrera);
        size_in_bytes += 5;
        size_in_bytes += 10;

        file.seekg((size_in_bytes * pos) + (1 * pos), ios::beg); // fixed length record
        file.read((char *)&record.codigo, 5);
        file.read((char *)&record.nombre, 11);
        file.read((char *)&record.apellidos, 20);
        file.read((char *)&record.carrera, 15);

        char ciclo_array[5];
        file.read((char *)&ciclo_array, 5);
        record.ciclo = atoi(ciclo_array);

        char mensualidad_array[10];
        file.read((char *)&mensualidad_array, 10);
        record.mensualidad = atof(mensualidad_array);

        file.get();
        file.close();

        return record;
    }

    bool Delete(int pos)
    {
    }
};

int main()
{
    FixedRecord fix("datos1.dat");
    /*
    Alumno record;
    record.inputData();
    fix.add(record);

     */
    /*
    vector<Alumno> alumnos = fix.load();
    for (auto item : alumnos)
        item.printAlumno();
        */

    Alumno readed = fix.readRecord(4);
    readed.printAlumno();
}