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
    private:
        float oTime;
    };

} // namespace BlueMarble
