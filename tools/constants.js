/**
 * Copyright 2021, Dmytro Kalpakchi
 * A JS script to get names and values of universal features (from https://universaldependencies.org/u/feat/all.html)
 * to generate C++ code for constants.h
 * NOTE: after doing
 */

// script 1
document.querySelectorAll('h2').forEach(function(x) {
  var code = x.querySelector('code');
  if (code != null && code !== undefined) {
    var name = x.textContent.split(": ")[0];
    name = name.replaceAll("[", "__").replaceAll("]", "").replaceAll(" ", "_");
    console.log('const struct UniversalToken ' + name.toUpperCase() + '_UFEAT = { "' + x.textContent.replace(": ", '", "') + '" };')
  }
});

// script 2
document.querySelectorAll('h3').forEach(function(x) {
  var code = x.querySelector('code');
  if (code != null && code !== undefined) {
    var name = x.textContent.split(": ")[0];
    name = name.replaceAll("[", "__").replaceAll("]", "").replaceAll(" ", "_");

    var elem = x;
    while (elem.nodeName != "H2") {
      elem = elem.previousElementSibling;
    }
    var feat = elem.textContent.split(": ")[0]
    feat = feat.replaceAll("[", "__").replaceAll("]", "").replaceAll(" ", "_");

    console.log('const struct UniversalToken ' + feat.toUpperCase() + "__" + name.toUpperCase() + '_UVAL = { "' + x.textContent.replace(": ", '", "') + '" };')
  }
});

// script 3
// `feats` can be obtained using script 1 for pushing names to the feats array instead of logging to console
// Used on this page: https://universaldependencies.org/ext-feat-index.html
var lang_spec = {}
document.querySelectorAll('h2').forEach(function(x) {
  var name = x.textContent.split(": ")[0];
  name = name.replaceAll("[", "__").replaceAll("]", "").replaceAll(" ", "_");
  if (!feats.includes(name)) {
    var ul = x.nextElementSibling.nextElementSibling;
    ul.querySelectorAll('li').forEach(function(li) {
      var data = li.textContent.split("\t");
      var val = data[0].split("=")[1];
      var lang = data[1].split("_")[0];
      if (!(lang in lang_spec))
        lang_spec[lang] = {};
      if (!(name in lang_spec[lang]))
        lang_spec[lang][name] = []
      if (!lang_spec[lang][name].includes(val))
        lang_spec[lang][name].push(val);
    });
  }
});
