"#version 330 core
"
"in vec3 vColorOut;  // 顶点着色器传递的颜色
"in vec2 texcoord;    // 纹理坐标
"
"out vec4 fColor;    // 片元输出像素的颜色
"
"uniform sampler2D Texture;  // 纹理图片
"
"void main()
"{
"    //fColor.rgb =  texture2D(Texture, texcoord.st).rgb;
"    fColor.rgb =  texture2D(Texture, vec2(texcoord.s, 1.0 - texcoord.t)).rgb;
"    //fColor.rgb = vec3(1, 0, 0);
"}
"