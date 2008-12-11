<?php
// $Id$
/**
 * @author Gerd Riesselmann
 * @author Jeff Warrington (jaydub) is new maintainer March 2008
 * @author Chris Nutting <Chris.Nutting@openx.org>
 * @author Bruno Massa
 * @file
 * Module settings.
 */

/**
 * Module settings page form.
 */
function _openx_settings() {
  $zones_num = variable_get('openx_num_zones', 3);
  $form['openx_num_zones'] = array(
    '#default_value'  => $zones_num,
    '#description'    => t('How many OpenX zones (blocks) do you wish to display?'),
    '#title'          => t('OpenX Zones'),
    '#type'           => 'textfield',
    '#size'           => 6,
  );

  $form['openx_zones'] = array(
    '#description'  => t("<ul>
        <li>The zone id can be found in the OpenX administration interface.</li>
        <li>The name can be any alpha-numeric string, this will be used for the block name.</li>
    </ul>"),
    '#theme'        => 'openx_settings_zones',
    '#title'        => t('OpenX blocks'),
    '#tree'         => TRUE,
    '#type'         => 'fieldset',
  );
  $zones = variable_get('openx_zones', array());
  for ($index = 0; $zones_num; $index++) {
    $form['openx_zones'][$index]['id'] = array(
      '#type'           => 'textfield',
      '#default_value'  => $zones[$index]['id'],
      '#size'           => 6,
    );
    $form['openx_zones'][$index]['name'] = array(
      '#type'           => 'textfield',
      '#default_value'  => $zones[$index]['name'],
      '#size'           => 20,
    );
  }

  $form['adserver'] = array(
    '#type'           => 'fieldset',
    '#title'          => t('OpenX server'),
    '#description'    => t('These settings are located on your OpenX server at <strong>Settings -> Main Settings -> Delivery Settings</strong><br />...or leave blank to use an OpenX hosted account'),
  );
  $form['adserver']['openx_delivery_url'] = array(
    '#type'           => 'textfield',
    '#default_value'  => variable_get('openx_delivery_url', 'd.openx.org'),
    '#title'          => t('OpenX delivery url'),
    '#description'    => t('For example "ads.example.org/delivery". "http://" is automatically prefixed.'),
    '#required'       => TRUE,
  );
  $form['adserver']['openx_delivery_url_https'] = array(
    '#type'           => 'textfield',
    '#default_value'  => variable_get('openx_delivery_url_https', 'd.openx.org'),
    '#title'          => t('OpenX https delivery url'),
    '#description'    => t('For example "ads.example.org/delivery". "https://" is automatically prefixed.'),
    '#required'       => TRUE,
  );

  return system_settings_form($form);
}

/**
 * Theme the OpenX blocks list as a table
 */
function theme_openx_settings_zones($form) {
  $header = array(t('Index'), t('Zone ID'), t('Block name'));
  foreach (element_children($form) as $zone) {
    $rows[] = array($zone, drupal_render($form[$zone]['id']), drupal_render($form[$zone]['name']));
  }
  $output .= theme('table', $header, $rows);

  return $output;
}
