#ifndef ILESSON_H
#define ILESSON_H

#include "ICalendarEntry.h"
// Abstrakcyjna klasa bazowa dla wszystkich lekcji (korepetycje i USOS)
// Implementuje wspólną logikę: sprawdzanie nakładania się i obliczanie końca lekcji
class ILesson : public ICalendarEntry {
public:
    virtual ~ILesson() = default;

    bool overlaps(ICalendarEntry &other) override;
    time_t getEndTime();

    // Gettery
    int getId() const { return id; }
    time_t getTimestamp()const { return timestamp; }
    int getDuration() const { return duration; }

    // Settery
    void setId(int v) { id = v; }
    void setTimestamp(time_t v) { timestamp = v; }
    void setDuration(int v) { duration = v; }

protected:
    int id = 0;
    time_t timestamp = 0;
    int duration = 0;
};

#endif
