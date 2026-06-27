/**
 * @file Cell.cpp
 * @brief 细胞类实现
 * @author 3
 */

#include "Cell.h"

Cell::Cell() : m_state(State::Dead), m_age(0) {}

Cell::State Cell::state() const {
    return m_state;
}

int Cell::age() const {
    return m_age;
}

void Cell::tick() {
    if (m_state == State::Alive) {
        m_age++;
    }
}

void Cell::kill() {
    m_state = State::Dead;
    m_age = 0;
}

void Cell::birth() {
    m_state = State::Alive;
    m_age = 1;
}

bool Cell::isAlive() const {
    return m_state == State::Alive;
}
