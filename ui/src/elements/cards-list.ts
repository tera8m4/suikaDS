import { html } from 'lit'
import { customElement } from 'lit/decorators.js'
import { MessageController } from './message-controller';
import { Element } from "./element";
import { AnkiTransport } from "../anki-transport";
import { Config } from '../config';
import "./card";

@customElement("cards-list")
export class CardsList extends Element {


  private messagesController: MessageController = new MessageController(this);

  createRenderRoot() { return this; }

  onUpdateCard(cardID: number) {
    // TODO: Check for an empty picture field and notifu a user
    const card = this.messagesController.message.find(x => x.id == cardID);
    if (card?.image) {
      AnkiTransport.updateLastNote(card.image, Config.getInstance().getData().ankiPictureField);
    }
  }

  render() {
    return html`<div class="cards-list"><h2>Cards</h2>
    ${this.messagesController.message.map(x => html`<game-card .message=${x} />`)}
    </div>`;
  }
}
