# deferred shading
为了解决大量光源造成的渲染时间过长问题，可以使用延迟渲染。

## 延迟渲染怎么能够减少渲染的时间

首先，先看一下渲染的管线的伪代码：
<span id = "pseudocode"></span>
```c++
for(int i = 0; i < mesh.size(); ++i)
{
    for(int j = 0; j < light.size(); ++j)
    {
        for(int k = 0; k < triangle.size(); ++k)
        {
            renderTriangle(light[j]);
        }
    }
}
```

可见，每一个三角形都需要和每一个光源进行计算。但是实际上，这些大多都是无效计算。因为除了平行光，绝大部分光源（如点光源，聚光灯）范围都不会太大，能够照到的物体非常有限，只会对小部分像素产生影响。

延迟渲染的基本思想是：先确定渲染到屏幕上的像素，再用像素去做光照。
刚开始这一点有点让人费解：一个二维像素坐标怎么和三维空间的光源进行光照计算呢？

实际上，通过所谓的gbuffer，我们可以保存对应像素的三维坐标，表面材质的信息。这样就能进行光照计算了。

并且根据上面的描述，可知不使用延迟渲染，光线会和每一个fragment进行计算，如[上面伪代码](#pseudocode)所示。但是在延迟渲染中，伪代码如下：

```c++
for(int i = 0; i < fragments.size(); ++i)
{
    geometry_data = getMeshData(fragments[i]);
    for(int j = 0; j < lights.size(); ++j)
    {
        renderMesh(geometry_data, lights[j]);
    }
}
```
在8.1的代码中，延迟渲染只是减少了一部分fragmnet的计算（无法在像素中显示的fragment，应该是被深度测试抛弃的），这时候光照的计算已经从整个场景变为屏幕像素的计算了。

为了进一步进行优化，在8.2中提出了光体积的概念。所谓光体积就是光源能够影响到的范围。我们知道光照的强度会随着距离的的增大而减小，当距离增加到一定程度时，我们就认为该光源无法对该距离以外的区域产生效果。这样就能进一步减少计算（原本所有像素都会和每个光源进行计算-->受影响的像素和光照进行计算)

PS. 如果没有指定multisampling,pixel和fragment应该是一一对应的关系。因此一些文章说延迟渲染就是把本来在物空间（三维空间）进行光照计算放到了像空间（二维空间）进行处理。

在给出代码前，先对延迟渲染的过程进行简单归纳，延迟渲染可以分为两步走：

    1. geometry pass. 通过 multiple render targets 渲染出场景的位置，法向量，纹理等关键信息，并制作纹理贴图。
    2. lighting pass. 渲染一张屏幕大小的矩形，通过第一步得到的几何信息计算最后的光照结果。
   
## code 
### 光照强度计算
![](./threshold.jpg)

整个流程的伪代码如图所示，假设光照强度衰减到某个数字后($\frac{1}{256}$)，不会对像素产生影响，于是通过解二次方程，得到光照影响的最大距离。具体代码如下:

```c++
const float maxBrightness = std::fmaxf(std::fmaxf(lightColors[i].r, lightColors[i].g), lightColors[i].b);
float radius = (-linear + std::sqrt(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * quadratic);
```
我们把这个光体积的半径通过uniform的形式传入fragment shader。然后遍历每个光源，如果当前fragment的实际位置距离光源的长度小于radius，就计算该光照对这个fragment的影响。

### 真正的光体积代码
由于gpu并不擅长进行分支和循环，8.2的代码在实际运行中并不能有很大的性能提升。我们需要进一步的优化：在lightPass中，不渲染整个quad，而是渲染每一个光体积球
#### extra 
为了能顺利画额外的光照cube，把深度信息拷贝到默认的帧缓冲中。

```c++
        // 2.5. copy content of geometry's depth buffer to default framebuffer's depth buffer
        // ----------------------------------------------------------------------------------
        glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
        // blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
        // the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the 		
        // depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
        glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
```
