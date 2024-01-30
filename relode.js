/*
try {
  location.reload();
  const footer_panel = document.querySelector('.stick-footer-panel__link');
  if (footer_panel != null) {
    footer_panel.click();
  }
} catch (e) {
  document.write(e.name + ': ' + e.message);
  throw e
}
*/
try {
  location.reload()
} catch (e) {
  console.log("Javascript ERROR:", e.name + ': ' + e.message);
}
