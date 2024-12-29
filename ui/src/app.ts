import { html, css } from 'lit'
import { customElement, property, state } from 'lit/decorators.js'
import { Transport } from './transport';
import "./elements/cards-list";
import "./elements/modal/anki-settings";
import { Element } from "./elements/element";

@customElement("my-app")
export class App extends Element {
  static styles = css`
  .card-container {
    display: flex;
    flex-direction: column;
    align-items: center; /* Centers cards horizontally */
    gap: 16px; /* Adds spacing between cards */
  }
  :host {
    width: 100%;
    height: 100%;
  }
    `;
  @state()
  connected: boolean = false;

  @property()
  isSettingsOpen: boolean = false;

  constructor() {
    super();

    Transport.getInstance().addInitializeHandler(this, (data) => {
      document.title = data.title;
      this.connected = true;
    });

    Transport.getInstance().init();
  }

  onCloseSettings(): void {
    this.isSettingsOpen = false;
  }

  onOpenSettings(): void {
    this.isSettingsOpen = true;
  }

  protected render() {
    return html`
      <nav>
        <ul>
          <li><strong>suikaDS</strong>
        </ul>
        <ul>
          <li><button @click=${this.onOpenSettings}>Open settings</button></li>
        </ul>
      </nav>
    ${this.connected
        ? html`<cards-list/>`
        : html`<p>Conntecing</p>`
      }
      <anki-settings .isOpen=${this.isSettingsOpen} @onClose=${this.onCloseSettings}/>
    `;
  }
};
