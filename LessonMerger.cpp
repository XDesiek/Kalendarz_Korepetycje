#include "LessonMerger.h"
#include <algorithm>

std::vector<std::shared_ptr<ILesson>> LessonMerger::mergeAndSort(
    const std::vector<std::shared_ptr<ILesson>>& studentLessons,
    const std::vector<std::shared_ptr<ILesson>>& usosLessons)
{
    std::vector<std::shared_ptr<ILesson>> merged;
    merged.reserve(studentLessons.size() + usosLessons.size());

    merged.insert(merged.end(), studentLessons.begin(), studentLessons.end());
    merged.insert(merged.end(), usosLessons.begin(), usosLessons.end());

    std::sort(merged.begin(), merged.end(), [](const std::shared_ptr<ILesson>& a, const std::shared_ptr<ILesson>& b) {
        return a->getTimestamp() < b->getTimestamp();
    });

    return merged;
}
