#pragma once

namespace BlueMarble {

    class TimeStep
    {
    public:
        TimeStep(float time = 0.0f)
            : oTime(time)
        {}

        operator float() const { return oTime; }

        float GetSeconds() const { return oTime; }
        float GetMilliseconds() const { return oTime * 1000.0f; }

        TimeStep operator +(TimeStep ts) { return TimeStep(oTime + ts.GetSeconds()); }
        TimeStep& operator +=(TimeStep& ts) 
        { 
            oTime += ts.GetSeconds();
            return *this;
        }
    private:
        float oTime;
    };

} // namespace BlueMarble
