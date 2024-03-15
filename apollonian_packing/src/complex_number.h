#pragma once

#include <cmath>
class ComplexNumber {
  private:
    float real;
    float imag;

  public:
    ComplexNumber();
    ComplexNumber(float real, float imag);
    ComplexNumber(const ComplexNumber &c);
    void setReal(float real);
    void setImag(float imag);
    float getReal();
    float getImag();

    ComplexNumber add(ComplexNumber c);
    ComplexNumber subtract(ComplexNumber c);
    ComplexNumber multiply(ComplexNumber c);
    ComplexNumber scale(float scalar);
    ComplexNumber sqrt();

    float distance(ComplexNumber c);

    void operator=(ComplexNumber c);
    ComplexNumber operator+(ComplexNumber c);
    ComplexNumber operator-(ComplexNumber c);
    ComplexNumber operator*(ComplexNumber c);

    bool operator==(ComplexNumber c) {
        return this->real == c.real && this->imag == c.imag;
    }
};

ComplexNumber::ComplexNumber() {
    this->real = 0;
    this->imag = 0;
}

ComplexNumber::ComplexNumber(float real, float imag) {
    this->real = real;
    this->imag = imag;
}

ComplexNumber::ComplexNumber(const ComplexNumber &c) {
    this->real = c.real;
    this->imag = c.imag;
}

void ComplexNumber::setReal(float real) { this->real = real; }

void ComplexNumber::setImag(float imag) { this->imag = imag; }

float ComplexNumber::getReal() { return this->real; }

float ComplexNumber::getImag() { return this->imag; }

ComplexNumber ComplexNumber::add(ComplexNumber c) {
    ComplexNumber result;
    result.real = this->real + c.real;
    result.imag = this->imag + c.imag;
    return result;
}

ComplexNumber ComplexNumber::subtract(ComplexNumber c) {
    ComplexNumber result;
    result.real = this->real - c.real;
    result.imag = this->imag - c.imag;
    return result;
}

ComplexNumber ComplexNumber::scale(float scalar) {
    ComplexNumber result;
    result.real = this->real * scalar;
    result.imag = this->imag * scalar;
    return result;
}

ComplexNumber ComplexNumber::multiply(ComplexNumber c) {
    ComplexNumber result;
    result.real = this->real * c.real - this->imag * c.imag;
    result.imag = this->real * c.imag + this->imag * c.real;
    return result;
}

ComplexNumber ComplexNumber::sqrt() {
    ComplexNumber result;
    float r = std::sqrt(this->real * this->real + this->imag * this->imag);
    float theta = std::atan2(this->imag, this->real);
    result.real = std::sqrt(r) * std::cos(theta / 2);
    result.imag = std::sqrt(r) * std::sin(theta / 2);
    return result;
}

float ComplexNumber::distance(ComplexNumber c) {
    auto dx = this->real - c.real;
    auto dy = this->imag - c.imag;
    return std::sqrt(dx * dx + dy * dy);
}

void ComplexNumber::operator=(ComplexNumber c) {
    this->real = c.real;
    this->imag = c.imag;
}

ComplexNumber ComplexNumber::operator+(ComplexNumber c) {
    ComplexNumber result;
    result.real = this->real + c.real;
    result.imag = this->imag + c.imag;
    return result;
}

ComplexNumber ComplexNumber::operator-(ComplexNumber c) {
    ComplexNumber result;
    result.real = this->real - c.real;
    result.imag = this->imag - c.imag;
    return result;
}

ComplexNumber ComplexNumber::operator*(ComplexNumber c) {
    ComplexNumber result;
    result.real = this->real * c.real - this->imag * c.imag;
    result.imag = this->real * c.imag + this->imag * c.real;
    return result;
}
