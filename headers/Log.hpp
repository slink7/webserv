#ifndef LOG_HPP
#define LOG_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class Log {
public:
    enum Level {
        INFO = 1 << 0,
        DEBUG = 1 << 1,
        WARNING = 1 << 2,
        ERROR = 1 << 3,
		FUNCTION = 1 << 4,
		ALL = ~0
    };

	static Log& Instance() {
		static Log	instance;
		return (instance);
	}

	static int& Count() {
		static int	count = 0;
		return (count);
	}

    static Log& out(Level level = INFO) {
		Log&  instance = Instance()(level);
		if (instance.lock & instance.key) {
			return (instance << "[" << LevelToString(level) << "-" << Count()++ << "\e[0m] ");
		}
		return (instance);
    }
 
	static void AddLevel(Level level) {
		Log& instance = Instance();
		instance.lock = Level(instance.lock | level);
	}

	static void RemoveLevel(Level level) {
		Log& instance = Instance();
		instance.lock = Level(instance.lock & ~level);
	}

    static void SetLevel(Level level) {
		
		Instance().lock = level;
    }

    static void SetOutput(std::ostream* output_stream) {
		Log& instance = Instance();
        instance.output = output_stream;
    }

    template<typename T>
    Log& operator<<(const T& msg) {
        if ((lock & key) && output) {
            *output << msg;
        }
        return (*this);
    }

    Log& operator()(Level level) {
        key = level;
        return (*this);
    }

private:
	
    // Constructeur privé pour le singleton
    Log() : output(&std::cout), key(INFO), lock(ALL) {}

    static std::string LevelToString(Level level) {
        switch (level) {
            case INFO: return "\e[0;37mIN";
            case DEBUG: return "\e[0;32mDE";
            case WARNING: return "\e[0;33mWA";
            case ERROR: return "\e[0;31mER";
			case FUNCTION: return "\e[0;36mFU";
            default: return "  ";
        }
    }

    std::ostream* output;
    Level key;
    Level lock;
};

#endif