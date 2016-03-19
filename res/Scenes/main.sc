Version = 1.;
numEntities = 20;
Entity0 : 
{
  Name = "BOX";
  Position = [ 23.07, 21.2, -5.97 ];
  Rotation = [ 0.13, 359.95, 0.35 ];
  Scale = [ 1., 1., 1. ];
  numComponents = 3;
  Component0 : 
  {
    Type = "Graphics";
    Texturepath = "res/Texture/white.png";
    Normalpath = "res/Texture/normal_map.jpg";
    Meshpath = "res/models/box.obj";
    Color = [ 1., 1., 1. ];
  };
  Component1 : 
  {
    Type = "Directional";
    Color = [ 1., 1., 1. ];
    Intensity = 0.4;
    Direction = [ 1., 1., 1. ];
  };
  Component2 : 
  {
    Type = "Skybox";
    Color = [ 1., 1., 1. ];
    Files = [ "res/Texture/Skybox/standard/posx.png", "res/Texture/Skybox/standard/negx.png", "res/Texture/Skybox/standard/posy.png", "res/Texture/Skybox/standard/negy.png", "res/Texture/Skybox/standard/posz.png", "res/Texture/Skybox/standard/negz.png" ];
  };
};
Entity1 : 
{
  Name = "BOX";
  Position = [ 143.59, 197.35, -39.05 ];
  Rotation = [ 0., 0., 0. ];
  Scale = [ 1., 1., 1. ];
  numComponents = 1;
  Component0 : 
  {
    Type = "Graphics";
    Texturepath = "res/Texture/House.png";
    Normalpath = "res/Texture/normal_map.jpg";
    Meshpath = "res/models/box.obj";
    Color = [ 1., 1., 1. ];
  };
};
Entity2 : 
{
  Name = "BOX";
  Position = [ -169.82, 1., -14.16 ];
  Rotation = [ 1.68, 0.7, 359.03 ];
  Scale = [ 1., 1., 1. ];
  numComponents = 2;
  Component0 : 
  {
    Type = "Graphics";
    Texturepath = "res/Texture/white.png";
    Normalpath = "res/Texture/normal_map.jpg";
    Meshpath = "res/models/box.obj";
    Color = [ 1., 1., 1. ];
  };
  Component1 : 
  {
    Type = "Physics";
    Physicstype = "PTSPHERE";
    Mass = 1.;
  };
};
Entity3 : 
{
  Name = "BOX";
  Position = [ -112.24, 1., 49.01 ];
  Rotation = [ 266.73, 59.38, 256.04 ];
  Scale = [ 1., 1., 1. ];
  numComponents = 2;
  Component0 : 
  {
    Type = "Graphics";
    Texturepath = "res/Texture/white.png";
    Normalpath = "res/Texture/normal_map.jpg";
    Meshpath = "res/models/box.obj";
    Color = [ 1., 1., 1. ];
  };
  Component1 : 
  {
    Type = "Physics";
    Physicstype = "PTSPHERE";
    Mass = 1.;
  };
};
Entity4 : 
{
  Name = "BOX";
  Position = [ -52.44, 1., 57.36 ];
  Rotation = [ 286.12, 46.65, 119.39 ];
  Scale = [ 1., 1., 1. ];
  numComponents = 2;
  Component0 : 
  {
    Type = "Graphics";
    Texturepath = "res/Texture/white.png";
    Normalpath = "res/Texture/normal_map.jpg";
    Meshpath = "res/models/box.obj";
    Color = [ 1., 1., 1. ];
  };
  Component1 : 
  {
    Type = "Physics";
    Physicstype = "PTSPHERE";
    Mass = 1.;
  };
};
Entity5 : 
{
  Name = "TERRAIN";
  Position = [ 57.28, 147.55, 95.81 ];
  Rotation = [ 0., 45., 0. ];
  Scale = [ 1., 1., 1. ];
  numComponents = 3;
  Component0 : 
  {
    Type = "Terrain";
    Terrainpath = "res/Texture/firstheightmap.jpg";
    Texturepath = "res/Texture/white.png";
    Scale = [ 2., 0.2, 2. ];
    Centered = true;
  };
  Component1 : 
  {
    Type = "Graphics";
    Texturepath = "res/Texture/white.png";
    Normalpath = "res/Texture/normal_map.jpg";
    Meshpath = "res/models/box.obj";
    Color = [ 1., 1., 1. ];
  };
  Component2 : 
  {
    Type = "Physics";
    Physicstype = "PTSPHERE";
    Mass = 1.;
  };
};
Entity6 : 
{
  Name = "BOX";
  Position = [ 96.22, 32.05, 114.14 ];
  Rotation = [ 168.49, 333.54, 89.28 ];
  Scale = [ 1., 1., 1. ];
  numComponents = 2;
  Component0 : 
  {
    Type = "Graphics";
    Texturepath = "res/Texture/white.png";
    Normalpath = "res/Texture/normal_map.jpg";
    Meshpath = "res/models/box.obj";
    Color = [ 1., 1., 1. ];
  };
  Component1 : 
  {
    Type = "Physics";
    Physicstype = "PTSPHERE";
    Mass = 1.;
  };
};
Entity7 : 
{
  Name = "BOX";
  Position = [ 97.6, 32.2, 115.62 ];
  Rotation = [ 61.87, 319.22, 244.03 ];
  Scale = [ 1., 1., 1. ];
  numComponents = 2;
  Component0 : 
  {
    Type = "Graphics";
    Texturepath = "res/Texture/white.png";
    Normalpath = "res/Texture/normal_map.jpg";
    Meshpath = "res/models/box.obj";
    Color = [ 1., 1., 1. ];
  };
  Component1 : 
  {
    Type = "Physics";
    Physicstype = "PTSPHERE";
    Mass = 1.;
  };
};
Entity8 : 
{
  Name = "BOX";
  Position = [ 184.83, 1.65, 117.05 ];
  Rotation = [ 300.04, 359.46, 94.02 ];
  Scale = [ 1., 1., 1. ];
  numComponents = 2;
  Component0 : 
  {
    Type = "Graphics";
    Texturepath = "res/Texture/white.png";
    Normalpath = "res/Texture/normal_map.jpg";
    Meshpath = "res/models/box.obj";
    Color = [ 1., 1., 1. ];
  };
  Component1 : 
  {
    Type = "Physics";
    Physicstype = "PTSPHERE";
    Mass = 1.;
  };
};
Entity9 : 
{
  Name = "BOX";
  Position = [ 213.91, 1.3, 125.75 ];
  Rotation = [ 300.28, 325.09, 148.87 ];
  Scale = [ 1., 1., 1. ];
  numComponents = 2;
  Component0 : 
  {
    Type = "Graphics";
    Texturepath = "res/Texture/white.png";
    Normalpath = "res/Texture/normal_map.jpg";
    Meshpath = "res/models/box.obj";
    Color = [ 1., 1., 1. ];
  };
  Component1 : 
  {
    Type = "Physics";
    Physicstype = "PTSPHERE";
    Mass = 1.;
  };
};
Entity10 : 
{
  Name = "BOX";
  Position = [ 159.5, 3.8, 123.28 ];
  Rotation = [ 358.14, 359.43, 0.03 ];
  Scale = [ 1., 1., 1. ];
  numComponents = 2;
  Component0 : 
  {
    Type = "Graphics";
    Texturepath = "res/Texture/white.png";
    Normalpath = "res/Texture/normal_map.jpg";
    Meshpath = "res/models/box.obj";
    Color = [ 1., 1., 1. ];
  };
  Component1 : 
  {
    Type = "Physics";
    Physicstype = "PTSPHERE";
    Mass = 1.;
  };
};
Entity11 : 
{
  Name = "BOX";
  Position = [ 121.23, 13.79, 172.43 ];
  Rotation = [ 256.14, 2.94, 74.32 ];
  Scale = [ 1., 1., 1. ];
  numComponents = 2;
  Component0 : 
  {
    Type = "Graphics";
    Texturepath = "res/Texture/white.png";
    Normalpath = "res/Texture/normal_map.jpg";
    Meshpath = "res/models/box.obj";
    Color = [ 1., 1., 1. ];
  };
  Component1 : 
  {
    Type = "Physics";
    Physicstype = "PTSPHERE";
    Mass = 1.;
  };
};
Entity12 : 
{
  Name = "BOX";
  Position = [ 183.83, 1.01, 110.16 ];
  Rotation = [ 210.91, 357.76, 270.76 ];
  Scale = [ 1., 1., 1. ];
  numComponents = 2;
  Component0 : 
  {
    Type = "Graphics";
    Texturepath = "res/Texture/white.png";
    Normalpath = "res/Texture/normal_map.jpg";
    Meshpath = "res/models/box.obj";
    Color = [ 1., 1., 1. ];
  };
  Component1 : 
  {
    Type = "Physics";
    Physicstype = "PTSPHERE";
    Mass = 1.;
  };
};
Entity13 : 
{
  Name = "BOX";
  Position = [ 179.18, 1., 102.13 ];
  Rotation = [ 350.11, 8.24, 80.87 ];
  Scale = [ 1., 1., 1. ];
  numComponents = 2;
  Component0 : 
  {
    Type = "Graphics";
    Texturepath = "res/Texture/white.png";
    Normalpath = "res/Texture/normal_map.jpg";
    Meshpath = "res/models/box.obj";
    Color = [ 1., 1., 1. ];
  };
  Component1 : 
  {
    Type = "Physics";
    Physicstype = "PTSPHERE";
    Mass = 1.;
  };
};
Entity14 : 
{
  Name = "BOX";
  Position = [ 180.15, 1., 99.74 ];
  Rotation = [ 248.2, 317.07, 112.59 ];
  Scale = [ 1., 1., 1. ];
  numComponents = 2;
  Component0 : 
  {
    Type = "Graphics";
    Texturepath = "res/Texture/white.png";
    Normalpath = "res/Texture/normal_map.jpg";
    Meshpath = "res/models/box.obj";
    Color = [ 1., 1., 1. ];
  };
  Component1 : 
  {
    Type = "Physics";
    Physicstype = "PTSPHERE";
    Mass = 1.;
  };
};
Entity15 : 
{
  Name = "BOX";
  Position = [ 70.09, 1.58, 236.32 ];
  Rotation = [ 180.26, 70.92, 328.07 ];
  Scale = [ 1., 1., 1. ];
  numComponents = 2;
  Component0 : 
  {
    Type = "Graphics";
    Texturepath = "res/Texture/white.png";
    Normalpath = "res/Texture/normal_map.jpg";
    Meshpath = "res/models/box.obj";
    Color = [ 1., 1., 1. ];
  };
  Component1 : 
  {
    Type = "Physics";
    Physicstype = "PTSPHERE";
    Mass = 1.;
  };
};
Entity16 : 
{
  Name = "BOX";
  Position = [ 212.08, 1.31, 126.57 ];
  Rotation = [ 70.68, 63.64, 332.97 ];
  Scale = [ 1., 1., 1. ];
  numComponents = 2;
  Component0 : 
  {
    Type = "Graphics";
    Texturepath = "res/Texture/white.png";
    Normalpath = "res/Texture/normal_map.jpg";
    Meshpath = "res/models/box.obj";
    Color = [ 1., 1., 1. ];
  };
  Component1 : 
  {
    Type = "Physics";
    Physicstype = "PTSPHERE";
    Mass = 1.;
  };
};
Entity17 : 
{
  Name = "BOX";
  Position = [ 179.35, 1., 110.46 ];
  Rotation = [ 161.05, 289.48, 349.63 ];
  Scale = [ 1., 1., 1. ];
  numComponents = 2;
  Component0 : 
  {
    Type = "Graphics";
    Texturepath = "res/Texture/white.png";
    Normalpath = "res/Texture/normal_map.jpg";
    Meshpath = "res/models/box.obj";
    Color = [ 1., 1., 1. ];
  };
  Component1 : 
  {
    Type = "Physics";
    Physicstype = "PTSPHERE";
    Mass = 1.;
  };
};
Entity18 : 
{
  Name = "BOX";
  Position = [ 134.78, 13.6, 176.08 ];
  Rotation = [ 83.21, 88.87, 138.79 ];
  Scale = [ 1., 1., 1. ];
  numComponents = 2;
  Component0 : 
  {
    Type = "Graphics";
    Texturepath = "res/Texture/white.png";
    Normalpath = "res/Texture/normal_map.jpg";
    Meshpath = "res/models/box.obj";
    Color = [ 1., 1., 1. ];
  };
  Component1 : 
  {
    Type = "Physics";
    Physicstype = "PTSPHERE";
    Mass = 1.;
  };
};
Entity19 : 
{
  Name = "BOX";
  Position = [ 133.65, 13.64, 177.73 ];
  Rotation = [ 341.71, 26.46, 4.83 ];
  Scale = [ 1., 1., 1. ];
  numComponents = 2;
  Component0 : 
  {
    Type = "Graphics";
    Texturepath = "res/Texture/white.png";
    Normalpath = "res/Texture/normal_map.jpg";
    Meshpath = "res/models/box.obj";
    Color = [ 1., 1., 1. ];
  };
  Component1 : 
  {
    Type = "Physics";
    Physicstype = "PTSPHERE";
    Mass = 1.;
  };
};
