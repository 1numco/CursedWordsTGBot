#include "task.h"



Task::Task(std::string mss, std::string cht, std::string fN, std::string lN, std::int64_t id): message(mss), chat_title(cht),firstName(fN), lastName(lN),  id_m(id){

    }


bool operator == (const Task task1, const Task task2){
    return task1.message == task2.message;
}