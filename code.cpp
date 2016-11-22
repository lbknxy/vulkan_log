drawFrame() {

    long vAcquireS = MGetCurTimeStamp();  // get time : ms

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(m_DeviceBag.logicalDevice, swapchain,
                                            std::numeric_limits<uint64_t>::max(),
                                            imageAvailableSemaphore, VK_NULL_HANDLE,
                                            &imageIndex);

    long vAcquireE = MGetCurTimeStamp();
    LOGE("vacquire time = %d ms", vAcquireE - vAcquireS);


    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    // render
    CALL_VK(vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE));

    // submit end time, present start time
    long vSubmitE = MGetCurTimeStamp();

    LOGE("vsubmit time = %d ms", vSubmitE - vAcquireE);

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapchain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    // present 
    result = vkQueuePresentKHR(graphicsQueue, &presentInfo);

    // present end time
    long vPresentE = MGetCurTimeStamp();

    // log time of vkQueuePresentKHR  had spent...
    LOGE("vpresent time = %d ms", vPresentE - vSubmitE);

}


// get time
long MGetCurTimeStamp() {
    struct timeval time;
    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

// draw loop
while(...){

    drawFrame();

}