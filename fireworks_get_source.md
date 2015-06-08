# Как получить исходный код библиотеки? #

Его можно загрузить из репозитория по адресу: https://fireworks.googlecode.com/hg. Для работы с репозиторием, вы должны установить Mercurial SCM. Как это сделать в Windows XP я опишу ниже.

## Как установить Mercurial в Windows XP и загрузить исходный код? ##

  1. Качаем сайта http://mercurial.selenic.com/downloads/ нужный нам дистрибутив:

![http://wiki.fireworks.googlecode.com/hg/images/image1.jpg](http://wiki.fireworks.googlecode.com/hg/images/image1.jpg)

Я выбрал [Mercurial 1.7.5 MSI installer - x86 Windows - requires admin rights](http://mercurial.selenic.com/release/windows/mercurial-1.7.5-x86.msi). Этот дистрибутив без TortoiseHg (GUI оболочка под Windows для Mercurial). Он
устанавливает только hg, работающий с консоли. Этого вполне достаточно, чтобы
скачать исходники из репозитория.

> 2. Устанавливаем только что скаченный дистрибутив

![http://wiki.fireworks.googlecode.com/hg/images/image2.jpg](http://wiki.fireworks.googlecode.com/hg/images/image2.jpg)

> 3. После того, как все установилось, запускаем командную строку Windows (cmd.exe):

Сначало создадим папку, куда будет скопирован исходный код библиотеки. Я Выбрал папку c:\source\. Вы можете выбрать другой путь, но такой, чтобы в нём не было русских букв и пробелов (у MinGW, которым вы будете собирать потом проект с этим проблемы).

![http://wiki.fireworks.googlecode.com/hg/images/image3.jpg](http://wiki.fireworks.googlecode.com/hg/images/image3.jpg)

Для того, чтобы создать папку и перейти в неё, я выполнил следующие команды:

```
cd c:\
mkdir source
cd source
```

> 4. После этого скопируем команду на странице
http://code.google.com/p/fireworks/source/checkout и выполним её. Нужно скопировать и выполнить вот эту команду:
```
hg clone https://fireworks.googlecode.com/hg/ fireworks
```

![http://wiki.fireworks.googlecode.com/hg/images/image5.jpg](http://wiki.fireworks.googlecode.com/hg/images/image5.jpg)

> 5. После выполнения команды, у вас должна появиться папка с исходным кодом. Можно
попробовать собрать проект.

![http://wiki.fireworks.googlecode.com/hg/images/image6.jpg](http://wiki.fireworks.googlecode.com/hg/images/image6.jpg)