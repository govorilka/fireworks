# Introduction #

Геометрия - это объект позволяющий управлять такими свойствами графического примитива, как размер, способ позиционирования и выравнивание.


# Details #

Для объекта geometry можно задать следующие свойства:

### position : relative | absolute | fixed ###

> Свойство позволяет указать способ позиционирования элемента. Значение relative - укавыет, что примитив будет располагаться относительно примитива-родителя (группы, к которой он относится). Значение absolute позволяет привязать примитив к любому другогому примитиву, если он не является дочерним примитивов данного. Значение fixed указывает, что примитив будет располагаться относительно координат сцены.

> Значение по умолчанию для свойства: relative

### parent : {имя примитива} ###

> Указывает к какому примитиву привязать данный. Значение свойства учитывается только если значение position равно absolute

> Значение по умолчанию для свойства: нет

### halign : beforeleft | left | center | centerdock | right | afterright ###

> Задает горизонтальное выравнивание примитива относительно родителя. Значение centerdock выравнивает примитив по центру, при этом значение ширины примитива становится равно значению ширины родителя.

> Значение по умолчанию для свойства: left

### valign : beforetop | top | middle | middledock | bottom | afterbottom ###

> Задает вертикальное выравнивание примитива относительно родителя. Значение middledock выравнивает примитив по середине, при этом значение высоты примитива становится равно значению высоты родителя.

> Значение по умолчанию для свойства: top