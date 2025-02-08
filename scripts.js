window.addEventListener('scroll', function()
{
    var nav = document.querySelector('nav');
    
    if(window.scrollY > 500)
    {
        nav.classList.add('scroll');
    }
    else
    {
        nav.classList.remove('scroll');
    }
})