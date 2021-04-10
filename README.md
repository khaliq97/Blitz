# Blitz Web
A web engine for fun that can render a sub set of the HTML and CSS as well as a very early stage JavaScript interpreter.

## About
This is a project to build a somewhat HTML5 compatible browser and learn a ton about web rendering along the way. Everything is written from scratch and a good way to learn C++. Blitz is still very much at the earlier stages of development, so plently more to come.

## Features (Broadly)
* HTML Parsing
* CSS ENGINE
* Render Tree Construction
* Layout Box Painting
* Very much WIP JS interpreter

## Preview (Browser Window)

![A preview of the Blitz browser window](https://github.com/khaliq97/Blitz/blob/master/screenshots/1.png?raw=true)


## How do I build and run this?

Currently you can only build Blitz on Ubuntu Linux. A bit limited for now, yes I know.

## Bash
### Required Depenencies
```sudo apt-get install gtkmm-3.0```
<br/>
```sudo apt-get install libcurl4```
</br>

### Download
```git clone https://github.com/khaliq97/Blitz.git```

### Build
```cd Blitz/src```
<br/>
```make```

### Run 
```sudo chmod u+x blitz.sh```
<br/>
```./blitz <HTML File Path e.g. index.html>```

Hopefully that all builds and it will launch the browser window (well more of a preview for now). 

### References
HTML5 Specification
<br/>
CSS3 Specifcation (Tokenizing, Parsing, Layout Boxes, Selectors)