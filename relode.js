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
  console.log("### reload start ###")
  console.log("url before reload", location.href)
//  location.reload()
  location.replace(myOpenWeatherURL)
  console.log("url after reload", location.href)
  console.log("### reload finish ###")
} catch (e) {
  console.log("Javascript ERROR:", e.name + ': ' + e.message);
}
