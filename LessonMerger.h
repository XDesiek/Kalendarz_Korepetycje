#ifndef LESSONMERGER_H
#define LESSONMERGER_H

#include "ILesson.h"
#include <vector>
#include <memory>

class LessonMerger {
public:
    LessonMerger() = default;

    //pobiera i zwraca nowy wektor, który zawiera wszystkie lekcje
    std::vector<std::shared_ptr<ILesson>> mergeAndSort(
        const std::vector<std::shared_ptr<ILesson>>& studentLessons,
        const std::vector<std::shared_ptr<ILesson>>& usosLessons);
};

#endif // LESSONMERGER_H
