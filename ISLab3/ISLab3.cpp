// ISLab3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <stdio.h>
#include <string>
#include <time.h>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

//Популяция
#define POPULATION_SIZE 100

//Гены
const string GENES = "АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ,.?!";

//Цель слово "МИР"
const string TARGET = "МИР";

//Функция, генерирующая рандомные числа в заданном диапазоне 
int random_num(int start, int end)
{
    int range = (end - start) + 1;
    int random_int = start + (rand() % range);
    return random_int;
}

//Генерируем рандомный ген
char mutated_genes()
{
    int len = GENES.size();
    int r = random_num(0, len - 1);
    return GENES[r];
}

//Генерируем хромосому из генов размером, слову "МИР"
string create_gnome()
{
    int len = TARGET.size();
    string gnome = "";
    for (int i = 0; i < len; i++)
        gnome += mutated_genes();
    return gnome;
}

//Класс Individual
class Individual
{
public:
    string chromosome;
    int fitness;
    Individual(string chromosome);
    Individual mate(Individual parent2);
    int cal_fitness();
};

Individual::Individual(string chromosome)
{
    this->chromosome = chromosome;
    fitness = cal_fitness();
};

//Селекция и скрещивание
Individual Individual::mate(Individual par2)
{
    //Хромосома для потомства
    string child_chromosome = "";

    int len = chromosome.size();
    for (int i = 0; i < len; i++)
    {
        float p = random_num(0, POPULATION_SIZE) / POPULATION_SIZE;

        //Если вероятность меньше 0.45, то выбираем родителя 1 
        if (p < 0.45)
            child_chromosome += chromosome[i];

        //Если вероятность от 0.45 до 0.90, то выбираем родителя 2 
        else if (p < 0.90)
            child_chromosome += par2.chromosome[i];

        //В противном случае, вставляем рандомный ген(mutate)
        else
            child_chromosome += mutated_genes();
    }

    //Создаем хромосому для потомства
    return Individual(child_chromosome);
};


//Подсчитываем количество не верных букв в переменнной fitness
int Individual::cal_fitness()
{
    int len = TARGET.size();
    int fitness = 0;
    for (int i = 0; i < len; i++)
    {
        if (chromosome[i] != TARGET[i])
            fitness++;
    }
    return fitness;
};

//Перегрузка оператора
bool operator<(const Individual& ind1, const Individual& ind2)
{
    return ind1.fitness < ind2.fitness;
}


int main()
{
    srand((unsigned)(time(0)));
    setlocale(LC_ALL, "Russian");

    //Текущее поколение
    int generation = 0;

    vector<Individual> population;
    bool found = false;

    //Создаем начальную популяцию
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        string gnome = create_gnome();
        population.push_back(Individual(gnome));
    }

    while (!found)
    {
        //Сортировка потомков по fitness (количество ошибок в слове)
        sort(population.begin(), population.end());

        //Если fitness равен 0, то мы достигли слова "МИР"
        if (population[0].fitness <= 0)
        {
            found = true;
            break;
        }

        //В противном случае, создаем новое поколение для генерации 
        vector<Individual> new_generation;

        //10% идет на следующую генерацию
        int s = (10 * POPULATION_SIZE) / 100;
        for (int i = 0; i < s; i++)
            new_generation.push_back(population[i]);

        //50% из более подходящей популяцию будут скрещиватьс для потомства
        s = (90 * POPULATION_SIZE) / 100;
        for (int i = 0; i < s; i++)
        {
            int len = population.size();
            int r = random_num(0, 50);
            Individual parent1 = population[r];
            r = random_num(0, 50);
            Individual parent2 = population[r];
            Individual offspring = parent1.mate(parent2);
            new_generation.push_back(offspring);
        }
        population = new_generation;
        cout << "Генерация: " << generation << "\t";
        cout << "Слово: " << population[0].chromosome << "\t";
        cout << "Количество ошибок в слове: " << population[0].fitness << "\n";

        generation++;
    }
    cout << "Генерация: " << generation << "\t";
    cout << "Слово: " << population[0].chromosome << "\t";
    cout << "Количество ошибок в слове: " << population[0].fitness << "\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
