/**
 * VKTS - VulKan ToolS.
 *
 * The MIT License (MIT)
 *
 * Copyright (c) since 2014 Norbert Nopper
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef VKTS_TASKQUEUE_HPP_
#define VKTS_TASKQUEUE_HPP_

#include <vkts/vkts.hpp>

namespace vkts
{

class TaskQueueElement
{

public:

    ITaskSP task;

    double created;

    double used;

    double recycled;

    double send;

    double received;

    TaskQueueElement(const double time) :
        task(nullptr), created(time), used(-1.0), recycled(-1.0), send(-1.0f), received(-1.0f)
    {
    }

    ~TaskQueueElement()
    {
    }

};

class TaskQueue
{

private:

    std::vector<TaskQueueElement*> taskQueueElementCache;

    ThreadsafeQueue<TaskQueueElement*> queue;

    mutable std::mutex mutex;

    TaskQueueElement* getTaskQueueElement();

    void recycleTaskQueueElement(TaskQueueElement* taskQueueElement);

public:

    TaskQueue();
    TaskQueue(const TaskQueue& other) = delete;
    TaskQueue(TaskQueue&& other) = delete;
    virtual ~TaskQueue();

    TaskQueue& operator =(const TaskQueue& other) = delete;
    TaskQueue& operator =(TaskQueue && other) = delete;

    VkBool32 addTask(const ITaskSP& task);

    VkBool32 receiveTask(ITaskSP& task);

    void reset();

};

typedef std::shared_ptr<TaskQueue> TaskQueueSP;

} /* namespace vkts */

#endif /* VKTS_TASKQUEUE_HPP_ */
