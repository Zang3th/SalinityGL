#pragma once

namespace Engine
{
    class Timer
    {
        private:
            double _start, _current;

        public:
            explicit Timer(double time_msec);
            ~Timer() = default;
            void Update(double dt_msec);
            [[nodiscard]] bool CheckElapsedAndReset();
    };
}