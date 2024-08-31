
#ifndef TASK_H
#define TASK_H

#include <stdio.h>
#include <cstdint>
#include <iostream>
#include <string>

struct Task{
    std::string message;
    std::string chat_title;
    std::string firstName;
    std::string lastName;
    std::int64_t id_m;

    
    Task(std::string mss = " ", std::string cht = "-1", std::string fN = "None", std::string lN = "None", std::int64_t id = -1);

    friend bool operator == (const Task task1, const Task task2);

    //pattern builder помогает создать объект в зависимости от заданных в конструкторе аргументов в любом порядке
};


#endif