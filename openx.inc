<?php

/**
 * @author Gerd Riesselmann
 * @author Jeff Warrington (jaydub) is new maintainer March 2008
 * @author Chris Nutting <Chris.Nutting@openx.org>
 * @author Bruno Massa
 *
 * @file
 * Module settings.
 */

/**
 * Insert the JavaScript into the page's header.
 */
function _openx_javascript() {
  global $user;
  static $spc_code;

  // No need to add the header more than once
  if (!empty($spc_code)) {
    return;
  }

  if (!empty($_SERVER['HTTPS'])) {
    $protocol = 'https';
    $server = variable_get('openx_delivery_url_https', 'd.openx.org');
  }
  else {
    $protocol = 'http';
    $server = variable_get('openx_delivery_url', 'd.openx.org');
  }
  $url = $protocol . '://' . trim($server, '/') . '/spcjs.php';
  
  $vars = array();
  if ($site_vars = variable_get('openx_site_vars', FALSE)) {
    if (module_exists('token')) {
      $data = array('global' => NULL, 'user' => $user);
      
      $node = menu_get_object('node');
      
      if (isset($node)) {
        $data['node'] = $node;
      }
    }
    foreach ($site_vars as $var) {
      if (!empty($var['key'])) {
        if (module_exists('token')) {
          $vars[$var['key']] = token_replace($var['value'], $data, array('clear' => TRUE));
        }
        else {
          $vars[$var['key']] = $var['value'];
        }
      }
    }
  }
  $stickstr = '?';
  if (!empty($vars)) {
    $url .= $stickstr . drupal_http_build_query($vars);
    $stickstr = '&';
  }

  $cookies = array();
  if ($cookie_vars = variable_get('openx_cookie_vars', FALSE)) {
    foreach ($cookie_vars as $var) {
      if (!empty($var['key'])) {
        $cookies[$var['key']] = $var['value'];
      }
    }
    if (!empty($cookies)) {
      $vars['cookie'] = $cookies;
    }
  }
  if (!empty($cookies)) {
    foreach ( $cookies as $cookiekey => $cookievalue) {
      $url .= $stickstr;
      $stickstr = '&';
      $url .= $cookiekey;
      $url .= '="+';
      $url .= 'getCookie(\'';
      $url .= $cookievalue;
      $url .= "')+\"";
    }
  }
// todo: find a better way to read cookie
  $script = 'function getCookie(c_name) {';
  $script .= '    if (document.cookie.length > 0) {';
  $script .= '     c_start = document.cookie.indexOf(c_name + "=");';
  $script .= '    if (c_start != -1) {';
  $script .= '         c_start = c_start + c_name.length + 1;';
  $script .= '        c_end = document.cookie.indexOf(";", c_start);';
  $script .= '       if (c_end == -1) {';
  $script .= '          c_end = document.cookie.length;';
  $script .= '     }';
  $script .= '    return unescape(document.cookie.substring(c_start, c_end));';
  $script .= '     }';
  $script .= '}';
  $script .= '   return "";';
  $script .= '}';

  $script .= "document.write(unescape(\"%3Cscript async='true' src='";
  $script .= $url;
  $script .= "'%3E%3C/script%3E\"));";

  drupal_add_js($script,array('type' => 'inline', 'scope' => 'footer', 'weight' => 5));


  $zones = variable_get('openx_zones', array());
  $script = "  var OA_zones = {\n";

  $js_zones = array();
  foreach ($zones as $idx => $zone) {
    if (!empty($zone['id'])) {
      $js_zones[] = "    '" . $zone['name'] . "' : " . $zone['id'];
    }
  }
  $script .= implode(",\n", $js_zones);
  $script .= "\n  }";


  $spc_code = 1;
  drupal_add_html_head(array(
    '#type' => 'html_tag',
    '#tag' => 'script',
    '#attributes' => array(
      'type' => 'text/javascript',
    ),
    '#value' => $script,
    '#weight' => 1000,
  ), 'openx_zones');

  return $url;
}

/**
 * Return zone with given index
 */
function _openx_get_zone($index_or_key) {
  $zones = variable_get('openx_zones', array());
  if (isset($zones[$index_or_key])) {
    return $zones[$index_or_key];
  }
  else {
    foreach ($zones as $zone) {
      if ($zone['name'] == $index_or_key) {
        return $zone;
      }
    }
  }

  // There is no zone with such ID or name
  return FALSE;
}
