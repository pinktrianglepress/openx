<?php
// $Id$
/**
 * @author Gerd Riesselmann
 * @author Jeff Warrington (jaydub) is new maintainer March 2008
 * @author Chris Nutting <Chris.Nutting@openx.org>
 * @author Bruno Massa
 * @file
 * Openx Block display.
 */

/**
 * Insert the JavaScript into the page's header.
 */
function _openx_javascript() {
  if (!empty($_SERVER['HTTPS'])) {
    $protocol = 'https';
    $server = variable_get('openx_delivery_url_https', 'd.openx.org');
  }
  else {
    $protocol = 'http';
    $server = variable_get('openx_delivery_url', 'd.openx.org');
  }
  $url = $protocol .'://'. trim($server, '/') .'/spcjs.php';

  $zones = variable_get('openx_zones', array());
  $spc_code = "<script type='text/javascript'><!--// <![CDATA[\n  var OA_zones = {\n";

  $js_zones = array();
  foreach ($zones as $idx => $zone) {
    if (!empty($zone['id'])) {
      $js_zones[] = "    '". $zone['name'] ."' : ". $zone['id'];
    }
  }
  $spc_code .= implode(",\n", $js_zones);
  $spc_code .= "  }\n// ]]> --></script>\n<script type='text/javascript' src='{$url}'></script>";

  drupal_set_html_head($spc_code);
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
