- By HHZ
  - 主程序框架搭好了，可以直接在上面修改
  - 实验课用过的单一纹理相关函数封装到`TexLoader`类里面了，需要用到纹理的可以直接调用，如果需要其他纹理相关功能可以往里面加
  
- By LRC

  12.10

  - 天空盒完成，可以用test里的main函数测试

  - 效果差强人意，4条y轴方向的边能看到不协调(如下图灰线指出的地方)，但是感觉很难再改进了

    <img src="C:\Users\Eragon\AppData\Roaming\Typora\typora-user-images\1639142340496.png" alt="1639142340496" style="zoom:50%;" />

  - 还没和HHZ的传送带拼到一起，下次开会前争取拼好

  - 改了一点`TexLoader`类，加了几条滤波的参数，且把生成纹理的函数从`glTexImage2D`换成了`gluBuild2DMipmaps`以避免天空盒出现纹理躁动

- Contributors
  - LRC
  - HHZ