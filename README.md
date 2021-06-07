# Blitz Web
A web engine for fun that can render a sub set of HTML and CSS as well as a very early stage JavaScript interpreter.

## About
This is a project to build a HTML5 compatible browser and learn a ton about web rendering along the way. Everything is written from scratch and I'm still very much learning C++. Blitz is still at an early stage of development, so plently more work to be done.

## Features (Broadly)
* HTML + CSS Tree Construction
* Render Tree Construction
* Layout Box Painting
* Early Stage JS interpreter

## Browser Window Screenshot

![A preview of the Blitz browser window](https://github.com/khaliq97/Blitz/blob/master/screenshots/1.png?raw=true)


## How do I build and run this?

Currently you can only build Blitz on Ubuntu Linux (tested on 20.04):

### Required Depenencies
##### Required Libraries
```sudo apt install libgtkmm-3.0-dev```

```sudo apt install libcurl-dev```

```sudo apt install libcurlpp-dev```

```sudo apt install libfmt-dev```
##### Build Tools
```sudo apt-get install make```

```sudo apt-get install g++```


### Download
```git clone https://github.com/khaliq97/Blitz.git```

### Build
```cd Blitz/src```

```make```
### Run
```./blitz <HTML File Name>```
#### This will run the html page shown in the preview screenshot:
```./blitz ../res/index.html```
####