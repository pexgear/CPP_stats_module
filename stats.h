#pragma once

#include <map>
#include <string>
#include <vector>
#include <functional>

using std::string;

template <typename T>
class stats
{
    public:
        stats() {}

        stats(string _name, T _max_stat)
        {
            init(_name, _max_stat);
        }

        stats(string _name, T _min_stat, T _max_stat)
        {
            init(_name, _min_stat, _max_stat);
        }

        ~stats() 
        {
            name.clear();
            set_max_stat(-1);
            set_min_stat(-1);
            set_stat(-1);
        }

    public:
        virtual void init(string _name, T _max_stat)
        {
            init(_name, 0, _max_stat);
        }

        virtual void init(string _name, T _min_stat, T _max_stat)
        {
            name = _name;
            set_min_stat(_min_stat);
            set_max_stat(_max_stat);
            set_stat_unckecked(max_stat);
            on_stat_init(_min_stat, _max_stat);
        }

        virtual void reinit()
        {
            set_stat_unckecked(max_stat);
        }

        virtual void reset_to_min()
        {
            set_stat_unckecked(min_stat);
        }

        virtual void reset_to_max()
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

        virtual T get_min_stat() const
        {
            return min_stat;
        }

        virtual void set_max_stat(T _max_stat)
        {
            T old_value = max_stat;
            max_stat = _max_stat;
            on_stat_max_changed(old_value, max_stat);
        }

        virtual void set_min_stat(T _min_stat)
        {
            T old_value = min_stat;
            min_stat = _min_stat;
            on_stat_min_changed(old_value, min_stat);
        }

        virtual void set_stat_unckecked(T _stat)
        {
            set_stat_internal(_stat);
        }

        void set_stat_internal(T _stat)
        {
            int old_stat = stat;
            stat = _stat;

            if (old_stat != _stat)
            {
                on_stat_changed(old_stat, stat);
            }
            
            if (stat <= min_stat)
            {
                on_min_stat_reached();
            }
            else if (stat >= max_stat)
            {
                on_max_stat_reached();
            }
        }

        virtual bool is_max_stat_valid() const
        {
            return max_stat > min_stat;
        }

        virtual bool is_valid() const
        {
            return is_max_stat_valid() && name.empty();
        }		

        void set_on_changed_delegate(std::function<void(string, T, T)> _delegate)
        {
            on_changed_delegate = _delegate;
        }

        void set_on_min_changed_delegate(std::function<void(string, T, T)> _delegate)
        {
            on_min_changed_delegate = _delegate;
        }

        void set_on_max_changed_delegate(std::function<void(string, T, T)> _delegate)
        {
            on_max_changed_delegate = _delegate;
        }

        void set_on_init_delegate(std::function<void(string, T, T)> _delegate)
        {
            on_init_delegate = _delegate;
        }


        void set_on_max_delegate(std::function<void(string)> _delegate)
        {
            on_max_delegate = _delegate;
        }

        void set_on_min_delegate(std::function<void(string)> _delegate)
        {
            on_min_delegate = _delegate;
        }

        virtual void on_stat_changed(T old_value, T new_value) 
        {
            if (on_changed_delegate)
            {
                on_changed_delegate(name, old_value, new_value);
            }
        }

        virtual void on_stat_min_changed(T old_value, T new_value) 
        {
            if (on_min_changed_delegate)
            {
                on_min_changed_delegate(name, old_value, new_value);
            }
        }

        virtual void on_stat_max_changed(T old_value, T new_value) 
        {
            if (on_max_changed_delegate)
            {
                on_max_changed_delegate(name, old_value, new_value);
            }
        }

        virtual void on_stat_init(T _min_stat, T _max_stat) 
        {
            if (on_init_delegate)
            {
                on_init_delegate(name, _min_stat, _max_stat);
            }
        }

        virtual void on_min_stat_reached() 
        {
            if (on_min_delegate)
            {
                on_min_delegate(name);
            }
        }

        virtual void on_max_stat_reached() 
        {
            if (on_max_delegate)
            {
                on_max_delegate(name);
            }
        }

    private:
        string name;
        T min_stat = 0.0;
        T max_stat = 0.0;
        T stat = 0.0;    

    private:
        std::function<void(string, T, T)> on_changed_delegate;
        std::function<void(string, T, T)> on_min_changed_delegate;
        std::function<void(string, T, T)> on_max_changed_delegate;
        std::function<void(string, T, T)> on_init_delegate;
        std::function<void(string)> on_max_delegate;
        std::function<void(string)> on_min_delegate;
};

template <typename T>
class stats_set 
{
    public:
        stats_set() {};

        ~stats_set() {
            stats_map.clear();
        };

        virtual void init_stats(const std::vector<string> _names) 
        {
            for(const string& name : _names)
            {
                add_stat(name);
            }            
        };

        void add_stat(const string& name) 
        {
            if(stats_map.count(name) <= 0)
            {
                add_or_replace_stat(name);
            }
        };

        void add_stat(const string& name, T max) 
        {
            if(stats_map.count(name) <= 0)
            {
                add_or_replace_stat(name, max);
            }
        };

        void add_stat(const string& _name, T _min, T _max) 
        {
            if(stats_map.count(_name) <= 0)
            {
                add_or_replace_stat(_name, _min, _max);
            }
        };

        void add_or_replace_stat(const string& _name) 
        {
            add_or_replace_stat(_name, 0.0, 0.0); 
        };

        void add_or_replace_stat(const string& _name, T _max) 
        {
            add_or_replace_stat(_name, 0.0, _max);
        };

        void add_or_replace_stat(const string& _name, T _min, T _max) 
        {
            stats_map[_name] = stats<T>();

            stats_map[_name].set_on_changed_delegate([this]
            (std::string _name, T _old_value, T _new_value) {
                this->on_changed(_name, _old_value, _new_value);
            });
            stats_map[_name].set_on_min_changed_delegate([this]
            (std::string _name, T _old_value, T _new_value) {
                this->on_min_changed(_name, _old_value, _new_value);
            });
            stats_map[_name].set_on_max_changed_delegate([this]
            (std::string _name, T _old_value, T _new_value) {
                this->on_max_changed(_name, _old_value, _new_value);
            });
            
            stats_map[_name].set_on_init_delegate([this]
            (std::string _name, T _min_stat, T _max_stat) {
                this->on_init(_name, _min_stat, _max_stat);
            });
            
            stats_map[_name].set_on_min_delegate([this]
            (std::string _name) {
                this->on_min(_name);
            });
            stats_map[_name].set_on_max_delegate([this]
            (std::string _name) {
            this->on_max(_name);
            });

            stats_map[_name].init(_name, _min, _max);
        };

        stats<T>& get_stat_by_name(const string& _name)
        {
            return stats_map[_name];
        }

        void set_on_changed_delegate(const string& _name, std::function<void(string, T, T)> _delegate)
        {
            get_stat_by_name(_name).set_on_changed_delegate(_delegate);
        }

        void set_on_min_changed_delegate(const string& _name, std::function<void(string, T, T)> _delegate)
        {
            get_stat_by_name(_name).set_on_min_changed_delegate(_delegate);
        }

        void set_on_max_changed_delegate(const string& _name, std::function<void(string, T, T)> _delegate)
        {
            get_stat_by_name(_name).set_on_max_changed_delegate(_delegate);
        }

        void set_on_init_delegate(const string& _name, std::function<void(string, T, T)> _delegate)
        {
            get_stat_by_name(_name).set_on_init_delegate(_delegate);
        }

        void set_on_max_delegate(const string& _name, std::function<void(string)> _delegate)
        {
            get_stat_by_name(_name).set_on_max_delegate(_delegate);
        }

        void set_on_min_delegate(const string& _name, std::function<void(string)> _delegate)
        {
            get_stat_by_name(_name).set_on_min_delegate(_delegate);
        }

        virtual void on_changed(string _name, T _old_value, T _new_value) {};
        virtual void on_min_changed(string _name, T _old_value, T _new_value) {};
        virtual void on_max_changed(string _name, T _old_value, T _new_value) {};
        virtual void on_init(string _name, T _min_stat, T _max_stat) {};
        virtual void on_min(string _name) {};
        virtual void on_max(string _name) {};

        std::map<string, stats<T>> stats_map;
};