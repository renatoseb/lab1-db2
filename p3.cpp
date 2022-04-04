#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <climits>

using namespace std;

struct Alumno
{
    string Nombre;
    string Apellidos;
    string Carrera;
    float mensualidad;

    void print()
    {
        cout << Nombre << endl
             << Apellidos << endl
             << Carrera << endl
             << mensualidad << endl;
    }
};

ostream &operator<<(ostream &stream, Alumno &p)
{
    cout << "Begin \n";
    stream << p.Nombre << '|';
    stream << p.Apellidos << '|';
    stream << p.Carrera << '|';
    stream << to_string(p.mensualidad);
    stream << "\n";
    stream << flush;
    return stream;
}

class VariableRecord
{
private:
    string filename;

public:
    VariableRecord(string filename)
    {
        this->filename = filename;
    }

    // DONE
    vector<Alumno> load()
    {
        ifstream File(this->filename, ios::in);
        vector<Alumno> alumnos;
        if (!File.is_open())
            cout << "No se pudo abrir el archivo. \n";

        string line;

        // Read header
        getline(File, line);

        while (getline(File, line))
        {
            // Read line
            char *data = const_cast<char *>(line.c_str());
            data = strtok(data, "|");
            Alumno record;

            record.Nombre = data;
            data = strtok(NULL, "|");

            record.Apellidos = data;
            data = strtok(NULL, "|");

            record.Carrera = data;
            data = strtok(NULL, "|");

            record.mensualidad = atof(data);
            data = strtok(NULL, "|");

            alumnos.push_back(record);
        }

        return alumnos;
    }

    // DONE
    void add(Alumno record)
    {
        ofstream File(this->filename, ios::app | ios::out);
        if (!File.is_open())
            cout << "No se pudo abrir el archivo. \n";

        File << record;

        File.close();
    }

    // DONE
    Alumno readRecord(int pos)
    {
        ifstream File(this->filename, ios::app | ios::in);
        if (!File.is_open())
            cout << "No se pudo abrir el archivo. \n";

        string line;

        // Read header
        getline(File, line);

        // Move to the correct position
        for (int i = 0; i < pos; ++i)
            getline(File, line);

        // Read line
        char *data = const_cast<char *>(line.c_str());
        data = strtok(data, "|");
        Alumno record;

        record.Nombre = data;
        data = strtok(NULL, "|");

        record.Apellidos = data;
        data = strtok(NULL, "|");

        record.Carrera = data;
        data = strtok(NULL, "|");

        record.mensualidad = atoi(data);
        data = strtok(NULL, "|");

        return record;
    }
};

int main()
{
    VariableRecord file("datos3.txt");
    Alumno alumno1;
    alumno1 = file.readRecord(1);
    alumno1.print();

    vector<Alumno> alumnos = file.load();
    for (Alumno i : alumnos)
        i.print();

    Alumno alumno2;
    alumno2.Nombre = "Pedro";
    alumno2.Apellidos = "Castillo Perez";
    alumno2.Carrera = "None";
    alumno2.mensualidad = 45999.40;

    file.add(alumno2);
    return 0;
}