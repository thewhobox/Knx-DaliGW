#include "Arduino.h"
#include "OpenKNX.h"
#include "MessageQueue.h"

uint8_t MessageQueue::push(Message *msg)
{
    while(isLocked) ;
    isLocked = true;

    msg->next = nullptr;
    if(tail == nullptr)
    {
        head = msg;
        tail = msg;
        isLocked = false;
        return msg->id;
    }

    tail->next = msg;
    tail = msg;
    lastPush = millis();
    isLocked = false;
    
    return msg->id;
}

bool MessageQueue::pop(Message &msg)
{
    if(millis() - lastPush < 50) return false;
    if(lastPop != 0 && (millis() - lastPop < 200)) return false;

    unsigned long started = millis();
    while(isLocked && (millis() - started < 3000)) ;
    if(isLocked) return false;
    isLocked = true;

    if(head == nullptr)
    {
        isLocked = false;
        return false;
    }
    
    if(head == nullptr)
        lastPop = millis();
    else
        lastPop = 0;

    msg.addrtype = head->addrtype;
    msg.data = head->data;
    msg.id = head->id;
    msg.para1 = head->para1;
    msg.para2 = head->para2;
    msg.type = head->type;
    msg.wait = head->wait;

    Message *temp = head;

    if(head->next == nullptr)
    {
        head = nullptr;
        tail = nullptr;
    } else {
        head = head->next;
    }

    delete temp;

    isLocked = false;
    return true;
}

uint8_t MessageQueue::getNextId()
{
    currentId++;
    if(currentId == 0) currentId++;
    responses[currentId] = -200;
    return currentId;
}

void MessageQueue::setResponse(uint8_t id, int16_t value)
{
    responses[id] = value;
}

int16_t MessageQueue::getResponse(uint8_t id)
{
    if(id == 0) return -13;
    return responses[id];
}