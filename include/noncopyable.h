//
// Created by Svain on 2022/7/9.
//

#ifndef FASTWEBSERVER_NONCOPYABLE_H
#define FASTWEBSERVER_NONCOPYABLE_H
#pragma once

class noncopyable{
protected:
    noncopyable(){}
    ~noncopyable(){}

private:
    noncopyable(const noncopyable&);
    const noncopyable& operator=(const noncopyable&);
};

#endif //FASTWEBSERVER_NONCOPYABLE_H
