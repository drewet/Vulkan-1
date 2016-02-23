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

#include "TaskQueue.hpp"

namespace vkts
{

TaskQueueElement* TaskQueue::getTaskQueueElement()
{
    std::lock_guard<std::mutex> lockGuard(mutex);

    if (taskQueueElementCache.size() > 0)
    {
        auto taskQueueElement = taskQueueElementCache.back();

        taskQueueElementCache.pop_back();

        taskQueueElement->used = timeGetRaw();

        return taskQueueElement;
    }

    return new TaskQueueElement(timeGetRaw());
}

void TaskQueue::recycleTaskQueueElement(TaskQueueElement* taskQueueElement)
{
    if (!taskQueueElement)
    {
        return;
    }

    std::lock_guard<std::mutex> lockGuard(mutex);

    taskQueueElement->task = ITaskSP();

    taskQueueElement->recycled = timeGetRaw();

    taskQueueElementCache.push_back(taskQueueElement);
}

TaskQueue::TaskQueue() :
    taskQueueElementCache(), queue(), mutex()
{
}

TaskQueue::~TaskQueue()
{
    TaskQueueElement* taskQueueElement;

    do
    {
        taskQueueElement = nullptr;

        queue.take(taskQueueElement);

        if (taskQueueElement)
        {
            recycleTaskQueueElement(taskQueueElement);
        }

    }
    while (taskQueueElement != nullptr);

    //

    for (auto currentCacheElement : taskQueueElementCache)
    {
        delete currentCacheElement;
    }

    taskQueueElementCache.clear();
}

VkBool32 TaskQueue::addTask(const ITaskSP& task)
{
    auto taskQueueElement = getTaskQueueElement();

    if (!taskQueueElement)
    {
        return VK_FALSE;
    }

    taskQueueElement->task = task;

    taskQueueElement->send = timeGetRaw();

    queue.add(taskQueueElement);

    return VK_TRUE;
}

VkBool32 TaskQueue::receiveTask(ITaskSP& task)
{
    TaskQueueElement* taskQueueElement = nullptr;

    queue.waitAndTake(taskQueueElement);

    if (!taskQueueElement)
    {
        return VK_FALSE;
    }

    taskQueueElement->received = timeGetRaw();

    task = taskQueueElement->task;

    recycleTaskQueueElement(taskQueueElement);

    if (!task.get())
    {
        return VK_FALSE;
    }

    return VK_TRUE;
}

void TaskQueue::reset()
{
	TaskQueueElement* taskQueueElement = nullptr;

	while (!queue.empty())
	{
		queue.take(taskQueueElement);
	}
}

} /* namespace vkts */
