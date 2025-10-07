<!doctype html>
<html lang="ru">
<head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width,initial-scale=1" />
  <title>Рандомные котики 🐱</title>
  <style>
    :root{--bg:#0f1724;--card:#0b1220;--accent:#ffb86b;--glass:rgba(255,255,255,0.04)}
    *{box-sizing:border-box}
    html,body{height:100%;margin:0;font-family:Inter, ui-sans-serif, system-ui, -apple-system, "Segoe UI", Roboto, "Helvetica Neue", Arial;color:#e6eef8;background:linear-gradient(180deg,#071020 0%, #0b1530 100%);}
    .wrap{min-height:100%;display:flex;align-items:center;justify-content:center;padding:28px}
    .card{width:100%;max-width:960px;background:linear-gradient(180deg, rgba(255,255,255,0.02), rgba(255,255,255,0.01));border-radius:14px;padding:20px;box-shadow:0 8px 30px rgba(2,6,23,0.6);border:1px solid rgba(255,255,255,0.03)}
    header{display:flex;gap:16px;align-items:center;margin-bottom:14px}
    .logo{width:64px;height:64px;border-radius:12px;background:linear-gradient(135deg,#ffecd2,#ffb86b);display:flex;align-items:center;justify-content:center;font-weight:700;color:#0b1220;font-size:28px}
    h1{margin:0;font-size:20px}
    p.lead{margin:0;color:#bcd3ee;opacity:0.9}
    .image-area{margin-top:16px;border-radius:10px;overflow:hidden;background:var(--glass);display:flex;align-items:center;justify-content:center;min-height:360px;position:relative}
    .image-area img{max-width:100%;max-height:80vh;display:block}
    .controls{display:flex;gap:8px;margin-top:14px;flex-wrap:wrap}
    button{background:#071a2b;color:#e6eef8;border:1px solid rgba(255,255,255,0.04);padding:10px 12px;border-radius:10px;font-weight:600;cursor:pointer}
    button.secondary{background:transparent;border:1px solid rgba(255,255,255,0.06)}
    .muted{color:#9fb1d9;font-size:13px}
    .meta{display:flex;justify-content:space-between;align-items:center;margin-top:10px}
    .small{font-size:13px;color:#9fb1d9}
    .center{display:flex;align-items:center;gap:8px}
    .loader{position:absolute;inset:0;display:flex;align-items:center;justify-content:center;background:linear-gradient(180deg, rgba(2,6,23,0.4), rgba(2,6,23,0.2));backdrop-filter:blur(4px)}
    .hidden{display:none}
    .switch{display:inline-flex;align-items:center;gap:6px}
    input[type=range]{width:150px}
    footer{text-align:center;margin-top:16px;color:#9fb1d9;font-size:14px}
    footer a{color:#ffb86b;text-decoration:none;font-weight:600}
    footer a:hover{text-decoration:underline}
    @media (max-width:600px){.logo{width:48px;height:48px;font-size:20px}.image-area{min-height:240px}}
  </style>
</head>
<body>
  <div class="wrap">
    <div class="card">
      <header>
        <div class="logo">🐾</div>
        <div>
          <h1>Рандомные котики</h1>
          <p class="lead">Кнопка — и новый пушистый друг. Нажмите пробел для следующего.</p>
        </div>
      </header>

      <div class="image-area" id="imageArea">
        <div class="loader" id="loader">Загружаю котика...</div>
        <img id="catImage" alt="Random cat" class="hidden" />
      </div>

      <div class="controls">
        <button id="newBtn">Сюрприз мне 🐱</button>
        <button id="favBtn" class="secondary">❤ Добавить в избранное</button>
        <button id="downloadBtn" class="secondary">⬇ Скачать</button>

        <div style="flex:1"></div>

        <label class="switch small">Авто-амбассадор
          <input type="checkbox" id="slideshow" style="margin-left:6px" />
        </label>
        <label class="switch small">Интервал
          <input type="range" id="intervalRange" min="2" max="12" value="4" />
        </label>
      </div>

      <div class="meta">
        <div class="small">Источник: The Cat API (если доступен) / запасной генератор</div>
        <div class="center muted"><span id="status">Готов</span></div>
      </div>

      <footer>
        Создатель сайта: <a href="https://t.me/qqveins11" target="_blank">@qqveins11</a>
      </footer>

    </div>
  </div>

  <script>
    const catImage = document.getElementById('catImage');
    const loader = document.getElementById('loader');
    const newBtn = document.getElementById('newBtn');
    const favBtn = document.getElementById('favBtn');
    const downloadBtn = document.getElementById('downloadBtn');
    const status = document.getElementById('status');
    const slideshow = document.getElementById('slideshow');
    const intervalRange = document.getElementById('intervalRange');

    let currentUrl = '';
    let slideTimer = null;
    const favorites = [];

    async function fetchCat() {
      showLoader(true);
      status.textContent = 'Загружаю...';
      try {
        const res = await fetch('https://api.thecatapi.com/v1/images/search');
        if (!res.ok) throw new Error('cat api failed');
        const data = await res.json();
        if (data && data[0] && data[0].url) {
          currentUrl = data[0].url;
          showImage(currentUrl);
          status.textContent = 'Котик от The Cat API';
          return;
        }
        throw new Error('invalid data');
      } catch (e) {
        console.warn('The Cat API не доступен, используем запасной генератор', e);
        const w = 600 + Math.floor(Math.random()*600);
        const h = 400 + Math.floor(Math.random()*400);
        currentUrl = `https://placekitten.com/${w}/${h}`;
        showImage(currentUrl);
        status.textContent = 'Запасной источник: placekitten.com';
      } finally {
        showLoader(false);
      }
    }

    function showImage(url) {
      catImage.classList.add('hidden');
      catImage.src = url;
      catImage.onload = () => {
        catImage.classList.remove('hidden');
      }
    }

    function showLoader(v) {
      loader.style.display = v ? 'flex' : 'none';
    }

    newBtn.addEventListener('click', fetchCat);

    favBtn.addEventListener('click', () => {
      if (!currentUrl) return alert('Сначала загрузите картинку.');
      if (!favorites.includes(currentUrl)) {
        favorites.push(currentUrl);
        favBtn.textContent = '❤ В избранном (' + favorites.length + ')';
      } else {
        alert('Уже в избранном!');
      }
    });

    downloadBtn.addEventListener('click', () => {
      if (!currentUrl) return alert('Нет картинки для скачивания.');
      const a = document.createElement('a');
      a.href = currentUrl;
      const ext = currentUrl.split('.').pop().split('?')[0];
      a.download = 'cat.' + ext;
      document.body.appendChild(a);
      a.click();
      a.remove();
    });

    slideshow.addEventListener('change', () => {
      if (slideshow.checked) startSlideshow(); else stopSlideshow();
    });
    intervalRange.addEventListener('input', () => {
      if (slideshow.checked) {
        stopSlideshow(); startSlideshow();
      }
    });

    function startSlideshow() {
      const sec = Number(intervalRange.value);
      stopSlideshow();
      slideTimer = setInterval(fetchCat, sec * 1000);
      status.textContent = 'Слайды каждые ' + sec + ' с.';
    }
    function stopSlideshow() { if (slideTimer) clearInterval(slideTimer); slideTimer = null; }

    window.addEventListener('keydown', (e) => { if (e.code === 'Space') { e.preventDefault(); fetchCat(); } });

    fetchCat();
  </script>
</body>
</html>
