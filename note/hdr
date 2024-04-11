# hdr (high dynamic range)

## 动机
在帧缓冲中，颜色和亮度的值在0.0到1.0之间。但是在光栅化的过程中，这个取值是有可能超过1.0的。这时如果只是把所有超过1.0的像素值设置为1.0，会导致场景非常混乱，损失很多细节，如图所示:
![](./hdr_clamped.png)

为了解决这个问题，可以让颜色暂时超过1.0，然后将其映射到0.0到1.0之间。

hdr本来是用在摄影上，通过对同一个场景采取不同曝光时间，然后合成成为一张照片的方法，如下图所示：
![](./hdr_image.png)

左边这张图片在被光照亮的区域充满细节，但是在黑暗的区域就什么都看不见了；但是右边这张图的高曝光却可以让之前看不出来的黑暗区域显现出来。

## 实现

###  浮点帧缓冲
当帧缓冲使用了一个标准化的定点格式(像GL_RGB)为其颜色缓冲的内部格式，OpenGL会在将这些值存入帧缓冲前自动将其约束到0.0到1.0之间。这一操作对大部分帧缓冲格式都是成立的，除了专门用来存放被拓展范围值的浮点格式。

当一个帧缓冲的颜色缓冲的内部格式被设定成了GL_RGB16F, GL_RGBA16F, GL_RGB32F 或者GL_RGBA32F时，这些帧缓冲被叫做浮点帧缓冲(Floating Point Framebuffer)，浮点帧缓冲可以存储超过0.0到1.0范围的浮点值，所以非常适合HDR渲染。

```c++
glBindTexture(GL_TEXTURE_2D, colorBuffer);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);  
```
除非需要一个非常高的精确度，32位不是必须的，使用GLRGB16F就足够了。

### 色调映射(Tone Mapping)
色调映射是为了能将最终的颜色映射到0到1之间。以下介绍两种色调映射的方式。

#### Reinhard色调映射

```c++
void main()
{             
    const float gamma = 2.2;
    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;

    // Reinhard色调映射
    vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
    // Gamma校正
    mapped = pow(mapped, vec3(1.0 / gamma));

    color = vec4(mapped, 1.0);
}   
```

有了Reinhard色调映射的应用，我们不再会在场景明亮的地方损失细节。当然，这个算法是倾向明亮的区域的，暗的区域会不那么精细也不那么有区分度。

因为当hdrcolor为0.5时， mapped的值为$\frac{1}{3}$，也就是说暗色区域被挤压成了$\frac{1}{3}$，亮色区增加到了$\frac{2}{3}$。

#### 指数映射

```c++
uniform float exposure;

void main()
{             
    const float gamma = 2.2;
    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;

    // 曝光色调映射
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    // Gamma校正 
    mapped = pow(mapped, vec3(1.0 / gamma));

    color = vec4(mapped, 1.0);
} 
```
将exposure定义为默认为1.0的uniform，从而允许我们更加精确设定我们是要注重黑暗还是明亮的区域的HDR颜色值。举例来说，高曝光值会使隧道的黑暗部分显示更多的细节，然而低曝光值会显著减少黑暗区域的细节，但允许我们看到更多明亮区域的细节。下面这组图片展示了在不同曝光值下的通道：

![](./hdr_exposure.png)

如图所示，t越大，暗色部分所占颜色空间就越广，细节也就越多。
![](./exposure.jpg)