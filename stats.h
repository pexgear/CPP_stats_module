#pragma once

template <typename T>
class stats
{
    public:
        stats() {}

        stats(T _max_stat)
        {
            init(_max_stat);
        }

        ~stats() {}

    public:
        virtual void init(T _max_stat)
        {
            init_range(0.0, _max_stat);
        }

        virtual void init_range(T _min_stat, T _max_stat)
        {
            min_stat = _min_stat;
            max_stat = _max_stat;
            set_stat_unckecked(max_stat);
        }

        virtual void reinit()
        {
            set_stat_unckecked(max_stat);
        }

        virtual void set_stat(T _stat)
        {
            if (_stat < 0.0)
            {
                _stat = 0.0;
            }

            if (_stat > max_stat)
            {
                _stat = max_stat;
            }
            set_stat_internal(_stat);
        }

        virtual T get_stat() const
        {
            return stat;
        }

        virtual T get_max_stat() const
        {
            return max_stat;
        }

        virtual void set_stat_unckecked(T _stat)
        {
            set_stat_internal(_stat);
        }

        void set_stat_internal(T _stat)
        {
            stat = _stat;
            if (stat <= min_stat)
            {
                on_min_stat_reached();
            }
            else if (stat >= max_stat)
            {
                on_max_stat_reached();
            }
        }

        virtual bool is_max_stat_valid()
        {
            return max_stat > min_stat;
        }		

        virtual void on_min_stat_reached() {}
        virtual void on_max_stat_reached() {}

    private:
        T min_stat = 0.0;
        T max_stat = 0.0;
        T stat = 0.0;    
};
