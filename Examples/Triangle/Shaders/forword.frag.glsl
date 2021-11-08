// Copyright 2021 SMS
// License(Apache-2.0)

#version 450

in vec3 v_color;

void main()
{
  gl_FragColor = vec4(v_color, 1.0);
}