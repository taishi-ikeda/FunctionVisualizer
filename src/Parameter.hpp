#ifndef PARAMETER_HPP
#define PARAMETER_HPP

#include <QString>

class Parameter{

public:
    Parameter(const QString &name, 
              const double min, 
              const double max)
        : name_(name), min_(min), max_(max) {}

    bool operator==(const Parameter &other) const {
        constexpr double epsilon = 1e-9; // A small value for floating-point comparison
        return name_ == other.name_ && std::abs(min_ - other.min_) < epsilon && std::abs(max_ - other.max_) < epsilon;
    }

    QString get_name() const { return name_; }
    double get_min() const { return min_; }
    double get_max() const { return max_; }
    void set_min(double min) { min_ = min; }
    void set_max(double max) { max_ = max; }
    void set_name(const QString &name) { name_ = name; }

    

private:
    QString name_;
    double min_;
    double max_;
};

#endif // PARAMETER_HPP