#ifndef CELL_H
#define CELL_H

/**
 * @file Cell.h
 * @brief 细胞类定义
 * @author 3
 */

class Cell {
public:
    enum class State { 
        Dead,   ///< 死亡状态
        Alive   ///< 存活状态
    };
    
    Cell();

    State state() const;

    int age() const;

    void tick();

    void kill();

    void birth();
    
    bool isAlive() const;
    
private:
    State m_state;  ///< 细胞状态
    int m_age;      ///< 细胞年龄
};

#endif // CELL_H
