window.addEventListener('scroll', function()
{
    var nav = document.querySelector('nav');
    
    if(window.scrollY > 100)
    {
        nav.classList.add('scroll');
    }
    else
    {
        nav.classList.remove('scroll');
    }
})

async function fetchAndDisplayCode(file, elementId) {
    try {
        const response = await fetch(file);
        const code = await response.text();
        document.getElementById(elementId).textContent = code;
        Prism.highlightElement(document.getElementById(elementId));
    } catch (error) {
        console.error('Error fetching the file:', error);
    }
}

document.addEventListener('DOMContentLoaded', () => 
{
    fetchAndDisplayCode('code/CLAPP/ClaPP_ECSManager.cpp', 'code1');
    fetchAndDisplayCode('code/CLAPP/ClaPP_ECSManager.h', 'code2');
    fetchAndDisplayCode('code/CLAPP/ClaPP_SystemManager.cpp', 'code3');
    fetchAndDisplayCode('code/CLAPP/ClaPP_SystemManager.h', 'code4');
    fetchAndDisplayCode('code/CLAPP/ClaPP_ComponentManager.h', 'code5');
    fetchAndDisplayCode('code/CLAPP/ClaPP_EntityManager.h', 'code6');
});

function toggleCode(id)
{
    const code = document.getElementById(id);
    if(code.style.display === 'none')
    {
        code.style.display = 'block';
    }
    else
    {
        code.style.display = 'none';
    }
}

document.addEventListener('DOMContentLoaded' , () =>
{
    const descriptions = ["Engineer", "Designer", "Programmer"];
    let index = 0;

    setInterval(() =>
    {
        const span = document.querySelector("#changingText span");
        if (span)
        {
            span.style.transform = "translateY(10px)";
            span.style.opacity = 0;

            setTimeout(() =>
            {
                span.innerText = descriptions[index];
                span.style.transform = "translateY(0)";
                span.style.opacity = 1;
            }, 500); // Spend 0.5 seconds sliding the text

            index = (index + 1) % descriptions.length;
        }
    }, 1500); // Change text ever 1.5 seconds
});

// Adjusts mobile menu based on clicks
function AdjustMobileMenu() 
{
    let menuImg = document.getElementById("menu");

    // Set new menu button
    if(menuImg.src.includes("images/close.png"))
    {
        menuImg.src = "images/menu.png";
    }
    else
    {
        menuImg.src = "images/close.png";
    }
    // Move the menu button to the correct place
    menuImg.classList.toggle("moveMenu")
    // Turns off logo
    document.getElementById("logo").classList.toggle("disableDisplay");
    // Enable header text and background
    document.getElementById("navBar").classList.toggle("enableNavBk");
    document.getElementById("navBar").classList.toggle("enableNav");
}

// Click events for menu on mobile
document.getElementById("menu").addEventListener("click", AdjustMobileMenu)
document.querySelectorAll("nav ul li a").forEach(link => 
{
    link.addEventListener("click", AdjustMobileMenu);
})